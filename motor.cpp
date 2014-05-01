//*************************************************************************************
/** \file motor.cpp
 *    This file contains a very simple motor driver. 
 *
 *  Revisions:
 *    \li 4-14-14 EB and JF wrote this
 *    \li 4-16-14 JF added saturation limits to the power params
 *
 *  License:
 *    This file is copyright 2014 by FishBaconBerryOh and released under the Lesser GNU 
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
//*************************************************************************************

#include <stdlib.h>                         // Include standard library header files
#include <avr/io.h>			     // Include I/O Library

#include "rs232int.h"                       // Include header for serial port class
#include "motor.h"                   // Include header for the motor_driver class
#include "bit_twiddle.h"

//-------------------------------------------------------------------------------------
/** \brief This constructor sets up an motor
 *  \details motor function drives the PWM to the motor. The function \c set_power()
 * 	tells the motor how much torque to auint8_tpply. Sets the polarity. The function
 * 	\c brake() will apply damping to the motor to stop the motor.
 *  
 *  @param p_serial_port A pointer to the serial port which writes debugging info.
 *  @param pwm_ddr A pointer to the PWM data direction register
 *  @param pwm_bit A pointer to the PWM settings bit 
 *  @param mc_ddr A pointer to the motor controller data direction register
 *  @param mc_bit_a The first motor controller settings bit
 *  @param mc_bit_b The second motor controller settings bit 
 *  @param mc_bit_diag The motor controller output enable and diagnostic register
 *  @param mc_port The motor controller port
 *  @param ocr The compare register for setting the PWM duty cycle
 *  @param p_serial_port The debug output port
 */

motor::motor(volatile uint8_t* pwm_ddr, uint8_t pwm_bit, volatile uint8_t* mc_ddr, uint8_t mc_bit_a, uint8_t mc_bit_b, uint8_t mc_bit_diag,
			    volatile uint8_t* mc_port, volatile uint16_t* ocr, emstream* p_serial_port)
{
  mc_port_md = mc_port; // Save important member data, including port register addresses and bits.
  ocr_md = ocr;
  mc_bit_a_md = mc_bit_a;
  mc_bit_b_md = mc_bit_b;
  
  set_bit(*pwm_ddr, pwm_bit, 1);
  //*pwm_ddr |= (1<<pwm_bit); //Set motor 1 PWM pin as an output.
  set_bit(*mc_ddr, mc_bit_a, 1);
  set_bit(*mc_ddr, mc_bit_b, 1);
  //*mc_ddr |= (1<<mc_bit_a) | (1<<mc_bit_b); // Set motor 1 control pins as outputs.
  
  set_bit(*mc_ddr, mc_bit_diag, 0);
  //*mc_ddr &= ~(1<<mc_bit_diag); // Enable the internal pullup resistor for the Diagnostic Enable
  set_bit(*mc_port, mc_bit_diag, 1);
  //*mc_port |= (1<<mc_bit_diag); // pin of the motor controller.
  
  set_bit(TCCR1A, WGM11, 1);
  set_bit(TCCR1A, WGM10, 1);
  set_bit(TCCR1A, COM1A1, 1);
  set_bit(TCCR1A, COM1B1, 1);
  //TCCR1A |= (1<<WGM11) | (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1); // Set 10-bit Fast PWM mode, and configure
  
  set_bit(TCCR1A, COM1A0, 0);
  set_bit(TCCR1A, COM1B0, 0);
  //TCCR1A &= ~( (1<<COM1A0) | (1<<COM1B0) ) ;                     // the PWM pin to set at BOTTOM, and
                                                                   // clear at a compare match.
  TCCR1B = 0;
  set_bit(TCCR1B, WGM12, 1);
  set_bit(TCCR1B, CS11, 1);
  //TCCR1B = (1<<WGM12) | (1<<CS11); // Set Clock prescaler to 1/8
  
  //DBG (ptr_to_serial, "Motor constructor OK" << endl);
  
  brake(0);
	
}


//-------------------------------------------------------------------------------------
/** \brief This function sets the power level for the motor
 *  \details positive input drives positive torque and negative drives negative torque
 *  @param  power power level from -1023 to +1023. negative and positive indicate direction of motor spin
 *  @return none
 */

void motor::set_power (int16_t power)
{
  if(power >= 0)
  {
       
    set_bit(*mc_port_md, mc_bit_a_md, 1);
    //*mc_port_md |= (1<<mc_bit_a_md); //set to 10, or 01 or whatever is necessary use member data
    set_bit(*mc_port_md, mc_bit_b_md, 0); 
    //*mc_port_md &= ~(1<<mc_bit_b_md); // setting things on one of the ports
  }
  else
  {
    set_bit(*mc_port_md, mc_bit_a_md, 0);
    set_bit(*mc_port_md, mc_bit_b_md, 1); 
    //*mc_port_md |= (1<<mc_bit_b_md); //set to 10, or 01 or whatever is necessary use member data
    //*mc_port_md &= ~(1<<mc_bit_a_md); // setting things on one of the ports
    power = -power;
  }
  
  if(power>1023)
  {
    power = 1023;// This imposes a saturation based limiting of the motor power.
  }
  *ocr_md = power; //set the PWM compare register to the given power setting
}


//-------------------------------------------------------------------------------------
/** \brief This function applies damping to the motor to brake
 *  \details It creates a short between the two leads of the motor. Can vary the strength of damping
 *  @param power The power of the damping applied to the brake. 0 is freewheeling, 1023 is max damping
 *  @return none
 */

void motor::brake (uint16_t power)
{
  set_bit(*mc_port_md, mc_bit_a_md, 0);
  set_bit(*mc_port_md, mc_bit_b_md, 0);	
  //*mc_port_md &= ~( (1<<mc_bit_a_md)|(1<<mc_bit_b_md) );//set to 10, or 01 or whatever is necessary use member data

  if(power>1023) 
  {
    power = 1023;// This imposes a saturation based limiting of the brake power.
  }	
  // setting things on one of the ports
  *ocr_md = power;
}

