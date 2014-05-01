//**************************************************************************************
/** \file task_master.cpp
 *    this file implements a task that manages top level system behavior
 *
 *  Revisions:
 *    \li 04-17-2014 EB Copied from task_brightness
 *
 *  License:
 *    This file is copyright 2012 by JR Ridgely and released under the Lesser GNU 
 *    Public License, version 2. It intended for educational use only, but its use
 *    is not limited thereto. */
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUEN-
 *    TIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 *    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 *    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 *    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
//**************************************************************************************

#include "frt_text_queue.h"            // Header for text queue class
#include "task_master.h"              // Header for this task
#include "bit_twiddle.h"		//fxns to make bit operations more readable
#include "shares.h"                    // Shared inter-task communications


//-------------------------------------------------------------------------------------
/** The main job of this constructor is to call the constructor of parent class
 * (\c frt_task ); the parent's constructor does the work.
 *  @param a_name A character string which will be the name of this task
 *  @param a_priority The priority at which this task will initially run (default: 0)
 *  @param a_stack_size The size of this task's stack in bytes 
 *                      (default: configMINIMAL_STACK_SIZE)
 *  @param p_ser_dev Pointer to a serial device (port, radio, SD card, etc.) which can
 *                   be used by this task to communicate (default: NULL)
 */

task_master::task_master (const char* a_name,
							unsigned portBASE_TYPE a_priority, 
							size_t a_stack_size,
							emstream* p_ser_dev
														)
	: frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
{
	// Nothing is done in the body of this constructor. All the work is done in the
	// call to the frt_task constructor on the line just above this one
}


//-------------------------------------------------------------------------------------
/** This method is called once by the RTOS scheduler. Each time around the for (;;)
 *  loop, the top level behavior is mediated by passing info to/from lower-level tasks
 * via shared data.
 * 
 */

void task_master::run (void)
{
	// Make a variable which will hold times to use for precise task scheduling
	portTickType previousTicks = xTaskGetTickCount ();

    //shared_adc0_state.put(OFF);
    shared_adc1_state.put(READ_ONCE);
    //shared_adc2_state.put(OFF);
    shared_adc3_state.put(READ_ONCE);
    shared_motor1_state.put( RUN );
    shared_motor2_state.put( RUN );
    
	for (;;)
	{ 

		shared_motor1_power.put( shared_adc3_value.get() - 512);
		shared_motor2_power.put( shared_adc1_value.get() - 512);

		
		delay_from_to (previousTicks, configMS_TO_TICKS (100));

		
	}
}

