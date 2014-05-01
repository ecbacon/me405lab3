//*************************************************************************************
/** \file shares.h
 *    This file contains extern declarations for queues and other inter-task data
 *    communication objects used in a ME405/507/FreeRTOS project. 
 *
 *  Revisions:
 *    \li 09-30-2012 JRR Original file was a one-file demonstration with two tasks
 *    \li 10-05-2012 JRR Split into multiple files, one for each task plus a main one
 *    \li 10-29-2012 JRR Reorganized with global queue and shared data references
 *    \li 04-17-2014 EB added shared data for lab 3 \c task_motor()
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

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _SHARES_H_
#define _SHARES_H_

//-------------------------------------------------------------------------------------
// Externs:  In this section, we declare variables and functions that are used in all
// (or at least two) of the files in the data acquisition project. Each of these items
// will also be declared exactly once, without the keyword 'extern', in one .cpp file
// as well as being declared extern here. 

// This queue allows tasks to send characters to the user interface task for display.
extern frt_text_queue print_ser_queue;

/**
 * shared data to send motor state/power messages to motor_task from other tasks
 */

/**
 *  Enum for commanding motor state
 */
enum motor_state_enum { BRAKE=0, RUN };


/**
 * shared motor data
 */
// these vars defined in task_motor.cpp
extern shared_data<motor_state_enum> shared_motor1_state; ///state of motor 1
extern shared_data<motor_state_enum> shared_motor2_state; ///state of motor 2
extern shared_data<int16_t> shared_motor1_power; ///power to motor 1
extern shared_data<int16_t> shared_motor2_power; ///power to motor 2
extern shared_data<uint16_t> shared_motor1_braking; ///braking command for motor 1
extern shared_data<uint16_t> shared_motor2_braking; ///braking cmd for motor 2


/**
 * Enum for commanding adc state
 */
enum adc_state_enum { OFF=0, READ_ONCE, READ_AVERAGE, READ_MEDIAN };


/**
 * shared adc data
 */
 //these vars defined in task_adc.cpp
extern shared_data<adc_state_enum> shared_adc0_state; ///state of adc0
extern shared_data<adc_state_enum> shared_adc1_state; ///state of adc1
extern shared_data<adc_state_enum> shared_adc2_state; ///state of adc2
extern shared_data<adc_state_enum> shared_adc3_state; ///state of adc3
extern shared_data<uint16_t> shared_adc0_value;	///reading from adc0
extern shared_data<uint16_t> shared_adc1_value;	///reading from adc1
extern shared_data<uint16_t> shared_adc2_value;	///reading from adc2
extern shared_data<uint16_t> shared_adc3_value;	///reading from adc3

#endif // _SHARES_H_
