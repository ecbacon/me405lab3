//*************************************************************************************
/** \file lab_main.cpp
 *    This file contains the main() code for a program which runs the ME405 board for
 *    ME405 labs.

 *  Revisions:
 *    \li 04-17-2014 EB copied this from JRR's lab1_main
 *
 *  License:
 *		This file is copyright 2012 by JR Ridgely and released under the Lesser GNU 
 *		Public License, version 2. It intended for educational use only, but its use
 *		is not limited thereto. */
/*		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *		AND	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * 		IMPLIED 	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * 		ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * 		LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUEN-
 * 		TIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * 		OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * 		CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * 		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * 		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
//*************************************************************************************


#include <stdlib.h>                         // Prototype declarations for I/O functions
#include <avr/io.h>                         // Port I/O for SFR's
#include <avr/wdt.h>                        // Watchdog timer header
#include <string.h>                         // Functions for C string handling

#include "FreeRTOS.h"                       // Primary header for FreeRTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // FreeRTOS inter-task communication queues
#include "croutine.h"                       // Header for co-routines and such

#include "rs232int.h"                       // ME405/507 library for serial comm.
#include "time_stamp.h"                     // Class to implement a microsecond timer
#include "frt_task.h"                       // Header of wrapper for FreeRTOS tasks
#include "frt_text_queue.h"                 // Wrapper for FreeRTOS character queues
#include "frt_queue.h"                      // Header of wrapper for FreeRTOS queues
#include "frt_shared_data.h"                // Header for thread-safe shared data
#include "shares.h"                         // Global ('extern') queue declarations
#include "task_master.h"                // Header for the data acquisition task
#include "task_user.h"                      // Header for user interface task
#include "task_motor.h"
#include "task_adc.h"


// Declare the queues which are used by tasks to communicate with each other here. 
// Each queue must also be declared 'extern' in a header file which will be read 
// by every task that needs to use that queue. The format for all queues except 
// the serial text printing queue is 'frt_queue<type> name (size)', where 'type' 
// is the type of data in the queue and 'size' is the number of items (not neces-
// sarily bytes) which the queue can hold

/** This is a print queue, descended from \c emstream so that things can be printed 
 *  into the queue using the "<<" operator and they'll come out the other end as a 
 *  stream of characters. It's used by tasks that send things to the user interface 
 *  task to be printed. 
 */
frt_text_queue print_ser_queue (32, NULL, 10);




//=====================================================================================
/** The main function sets up the RTOS.  Some test tasks are created. Then the 
 *  scheduler is started up; the scheduler runs until power is turned off or there's a 
 *  reset.
 *  @return This is a real-time microcontroller program which doesn't return. Ever.
 */

int main (void)
{
	// Disable the watchdog timer unless it's needed later. This is important because
	// sometimes the watchdog timer may have been left on...and it tends to stay on
	MCUSR = 0;
	wdt_disable ();

	// Configure a serial port which can be used by a task to print debugging infor-
	// mation, or to allow user interaction, or for whatever use is appropriate.  The
	// serial port will be used by the user interface task after setup is complete and
	// the task scheduler has been started by the function vTaskStartScheduler()
	rs232 ser_port (9600, 1);
	ser_port << clrscr << PMS ("ME405 Lab 1 Starting Program") << endl;

	// Create the queues and other shared data items here
// 	print_ser_queue = new frt_text_queue (32, ser_port, 10);

	// The user interface is at low priority; it could have been run in the idle task
	// but it is desired to exercise the RTOS more thoroughly in this test program
	new task_user ("UserInt", task_priority (1), 260, &ser_port);

    new task_motor ("Motor", task_priority (1), 280, &ser_port);

    new task_adc ( "ADC", task_priority (2), 280, &ser_port );
    
	// Create a task which commands other tasks using shared data
	new task_master ("Master", task_priority (1), 280, &ser_port);

	// Here's where the RTOS scheduler is started up. It should never exit as long as
	// power is on and the microcontroller isn't rebooted
	vTaskStartScheduler ();
}

