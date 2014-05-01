//**************************************************************************************
/** \file task_motor.cpp
 *    this file defines a task that manages motor objects
 *
 *  Revisions:
 *    \li 04-17-2014 EB created this task by copying task_brightness.cpp
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

#include "frt_text_queue.h"    // Header for text queue class
#include "task_motor.h"        // Header for this task
#include "bit_twiddle.h"	//fxns that make bit operations more readable
#include "shares.h" 		// Shared inter-task communications 
//-------------------------------------------------------------------------------------


/**
 * These are the shared data for communicating from other tasks to task_motor.
 */

shared_data <motor_state_enum>	shared_motor1_state;	///the commanded state of motor1
/// motor_states enum declared in shares.h
/// motor states can be RUN or BRAKE
shared_data <motor_state_enum> shared_motor2_state;   ///commanded state of motor2
shared_data <int16_t>		shared_motor1_power;	///commanded motor 1 power
shared_data <int16_t>		shared_motor2_power;   
shared_data <uint16_t>		shared_motor1_braking;
shared_data <uint16_t>		shared_motor2_braking;




/** This constructor creates a task which controls motors.
 * The main job of this constructor is to call the constructor of parent 
 * class (\c frt_task ); the parent's constructor the work.
 *  @param a_name A character string which will be the name of this task
 *  @param a_priority The priority at which this task will initially run (default: 0)
 *  @param a_stack_size The size of this task's stack in bytes 
 *                      (default: configMINIMAL_STACK_SIZE)
 *  @param p_ser_dev Pointer to a serial device (port, radio, SD card, etc.) which can
 *                   be used by this task to communicate (default: NULL)
 */

task_motor::task_motor (	const char* a_name, 
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
 *  loop, it reads the shared motor settings and then commands the motors
 */

void task_motor::run (void)
{
	// Make a variable which will hold times to use for precise task scheduling
	portTickType previousTicks = xTaskGetTickCount ();
	
	//instantiate 2 motors.
        motor motor1(&DDRB, (uint8_t)PB6, &DDRC, (uint8_t)PC0, (uint8_t)PC1, (uint8_t)PC2, &PORTC, &OCR1B);
	motor motor2(&DDRB, (uint8_t)PB5, &DDRD, (uint8_t)PD5, (uint8_t)PD6, (uint8_t)PD7, &PORTD, &OCR1A);
	//motor1 is controlled outside of this task by "shared_motor1_state", using methods "get()" and "put()"
	//motor2 is controlled by "shared_motr2_state", the same way.
	
	
	for (;;)
	{ 
	        //if motor is commanded to run,...
               if ( shared_motor1_state.get() == RUN )
		  //...then send the motor the commanded power
		  motor1.set_power( shared_motor1_power.get() );
	       else
		  //...else it should be braking, so send the motor the brake command 
		  motor1.brake( shared_motor1_braking.get() );
	       
	       //same as motor 1
	       if ( shared_motor2_state.get() == RUN )
		  motor2.set_power( shared_motor2_power.get() ); 
	       else
		  motor2.brake( shared_motor2_braking.get() );
		

		// This is a method we use to cause a task to make one run through its task
		// loop every N milliseconds and let other tasks run at other times
		delay_from_to (previousTicks, configMS_TO_TICKS (100));		
		
	}
}

