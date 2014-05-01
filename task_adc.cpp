//**************************************************************************************
/** \file task_adc.cpp
 *    this file defines an adc manager task. 
 *
 *  Revisions:
 *    \li 04-23-2014 EB created this by copying from task_motor.cpp
 *
 *  License:
 *    This file was found fully written on the back of a napkin at dennys. As such, it
 * must be considered in the public domain. JR Ridgely has claimed he wrote this
 * software, and demands to license it under the lesser GNU pubic license, but his 
 * lawyers are not as good as my lawyers at lawying, so he will not win.
 */
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
#include "task_adc.h"        // Header for this task
#include "bit_twiddle.h"	//fxns that make bit operations more readable
#include "shares.h" 		// Shared inter-task communications 
//-------------------------------------------------------------------------------------

/**
 * These are the shared data for communicating ADC data to and fro among tasks.
 */
shared_data <adc_state_enum> shared_adc0_state;//adc state enums declared in shares.h
shared_data <adc_state_enum> shared_adc1_state;
shared_data <adc_state_enum> shared_adc2_state;//states are OFF, READ_ONCE, READ_AVERAGE, READ_MEDIAN
shared_data <adc_state_enum> shared_adc3_state;
shared_data <uint16_t> shared_adc0_value;
shared_data <uint16_t> shared_adc1_value;
shared_data <uint16_t> shared_adc2_value;
shared_data <uint16_t> shared_adc3_value;



/** This constructor creates a task which controls the brightness of an LED using
 *  input from an A/D converter. The main job of this constructor is to call the
 *  constructor of parent class (\c frt_task ); the parent's constructor the work.
 *  @param a_name A character string which will be the name of this task
 *  @param a_priority The priority at which this task will initially run (default: 0)
 *  @param a_stack_size The size of this task's stack in bytes 
 *                      (default: configMINIMAL_STACK_SIZE)
 *  @param p_ser_dev Pointer to a serial device (port, radio, SD card, etc.) which can
 *                   be used by this task to communicate (default: NULL)
 */
task_adc::task_adc (	const char* a_name, 
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
 *  loop, it reads the shared adc settings and, if necessary, sets adc output to the
 * shared data that exposes it to other tasks
 */

void task_adc::run (void)
{
	// Make a variable which will hold times to use for precise task scheduling
	portTickType previousTicks = xTaskGetTickCount ();
	
	//instantiate adc object.
        adc* p_my_adc = new adc (p_serial);	
	
	const uint8_t NUM_AVG_RDGS = 4;
	const uint8_t NUM_MED_RDGS = 15;
	
	for (;;)
	{ 
	        switch( shared_adc0_state.get() )
		{
		  case OFF:
		    break;
		    
		  case READ_ONCE:
		    shared_adc0_value.put( p_my_adc->read_once(0) );
		    break;
		  
		  case READ_AVERAGE:
		    shared_adc0_value.put( p_my_adc->read_average(0,NUM_AVG_RDGS) );
		    break;
		    
		  case READ_MEDIAN:
		    shared_adc0_value.put( p_my_adc->read_median(0,NUM_MED_RDGS) );
		    break;
		    
		  default:
		    break;
		    //this adc is off.
		}
		
		switch( shared_adc1_state.get() )
		{
		  case OFF:
		    break;
		    
		  case READ_ONCE:
		    shared_adc1_value.put( p_my_adc->read_once(1) );
		    break;
		  
		  case READ_AVERAGE:
		    shared_adc1_value.put( p_my_adc->read_average(1,NUM_AVG_RDGS) );
		    break;
		    
		  case READ_MEDIAN:
		    shared_adc1_value.put( p_my_adc->read_median(1,NUM_MED_RDGS) );
		    break;
		    
		  default:
		    break;
		    //this adc is off.
		}
		
		switch( shared_adc2_state.get() )
		{
		  case OFF:
		    break;
		    
		  case READ_ONCE:
		    shared_adc2_value.put( p_my_adc->read_once(2) );
		    break;
		  
		  case READ_AVERAGE:
		    shared_adc2_value.put( p_my_adc->read_average(2,NUM_AVG_RDGS) );
		    break;
		    
		  case READ_MEDIAN:
		    shared_adc2_value.put( p_my_adc->read_median(2,NUM_MED_RDGS) );
		    break;
		    
		  default:
		    break;
		    //this adc is off.
		}
		
		switch( shared_adc3_state.get() )
		{
		  case OFF:
		    break;
		    
		  case READ_ONCE:
		    shared_adc3_value.put( p_my_adc->read_once(3) );
		    break;
		  
		  case READ_AVERAGE:
		    shared_adc3_value.put( p_my_adc->read_average(3,NUM_AVG_RDGS) );
		    break;
		    
		  case READ_MEDIAN:
		    shared_adc3_value.put( p_my_adc->read_median(3,NUM_MED_RDGS) );
		    break;
		    
		  default:
		    break;
		    //this adc is off.
		}

		// This is a method we use to cause a task to make one run through its task
		// loop every N milliseconds and let other tasks run at other times
		delay_from_to (previousTicks, configMS_TO_TICKS (100));		
		
	}
}

