//*************************************************************************************
/** \file adc.cpp
 *    This file contains a very simple A/D converter driver. 
 *
 *  Revisions:
 *    \li 01-15-2008 JRR Original (somewhat useful) file
 *    \li 10-11-2012 JRR Less original, more useful file with FreeRTOS mutex added
 *    \li 10-12-2012 JRR There was a bug in the mutex code, and it has been fixed
 *    \li 04-09-2014 EB, MB, JF, SO Fixed the obfuscation added by JRR
 *    \li 04-16-2014 EB added \c read_median()
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
//*************************************************************************************
#define MAX_SAMPLES 62			     // Max number of samples to avg in read_oversampled()

#include <stdlib.h>                         // Include standard library header files
#include <avr/io.h>

#include "rs232int.h"                       // Include header for serial port class
#include "adc.h"                            // Include header for the A/D class



/** \brief This is a function that compares two numbers for sorting-- used in read_median 
 *  \details In \c read_median(), the median value of an array of ADC readings is found.
 * to find the median, the array must be sorted. the stdlib \c qsort() function requires
 * a function that performs the comparison on whatever data type it is sorting. This is
 * that function.
 *  
 *  @param num1 number to compare
 *  @param num2 the other number compared to
 */

int compare( const void * num1, const void * num2 )
{
	if( num1 > num2) return 1;
	if( num1 == num2)return 0;
	if( num1 < num2)return -1;
}


//-------------------------------------------------------------------------------------
/** \brief This constructor sets up an A/D converter. 
 *  \details The A/D is made ready so that when a method such as \c read_once() or 
 *  \c read_oversampled is called, correct A/D conversions can be performed. This is
 *      accomplished by enabling the ADC, setting the clock prescaler to "divide-by-32",
 *      and setting the analog reference voltage to AVCC with an external capacitor to 
 *      ground. Input should be a serial port. Successful construction is written to the
 *      debug.
 *  
 *  @param p_serial_port A pointer to the serial port which writes debugging info. 
 */

adc::adc (emstream* p_serial_port)
{
	ptr_to_serial = p_serial_port;

	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);// Set the "ADC Enable" bit within the ADC Status Register A,
	                 // and set the ADC clock prescaler to divide by 32.
	
	ADMUX |= 1<<REFS0;// Set the analog reference voltage to AVCC.
	
		
	DBG (ptr_to_serial, "A/D constructor OK" << endl);
}


//-------------------------------------------------------------------------------------
/** \brief This method takes one A/D reading from the given channel and returns it. 
 *  \details If the counter reaches a maximum number of cycles of 256, adc will timeout
 *  @param  ch The A/D channel which is being read must be from 0 to 7
 *  @return The result of the A/D conversion
 */

uint16_t adc::read_once (uint8_t ch)
{
	ADMUX &= ~( (1<<MUX2)|(1<<MUX1)|(1<<MUX0) ); // Clear the ADC Multiplexor Selection bits, and the set them to  
	ADMUX |= ch & ( (1<<MUX2)|(1<<MUX1)|(1<<MUX0) ); // the appropriate ADC channel, "ch".
	
	ADCSRA |= 1<<ADSC;// Set the "ADC start conversion" bit, initiating a conversion.
	
	const uint16_t maxWaitCycles = 60000;
	uint16_t counter = 0;
	
	while(ADCSRA & (1<<ADSC) && maxWaitCycles > counter++)// Poll the ADC start conversion bit, which will remain
                                // high until the conversion is complete.++++++++
	  ;
	
	return ADC;// return the resulting value found in the ADC register.
}


//-------------------------------------------------------------------------------------
/** \brief This method takes an average reading on an ADC
 *  \details Take an average reading from the adc that is on the channel input as a parameter.
 *      The number of samples to be averaged is determined by the parameter "samples". If the
 *      "samples" parameter exceeds the maximum permissible number of samples(63), then only
 *      63 samples are taken. The number of samples taken, and the oversampled value are 
 *      written to the debug stream. 
 *  @param channel
 *  @param samples
 *  @return adc Value
 */

uint16_t adc::read_average (uint8_t channel, uint8_t samples)
{
	uint16_t sum = 0;
	uint8_t count = 0;
	
	//throw away first value
	read_once(channel);
	
	for(count = 0; count<samples && count < MAX_SAMPLES ;count++)
	{   
	    sum += read_once(channel);
	}
	//Average the taken readings. Keep the result in the sum variable.
	return sum/count;
}


/** \brief This method takes a median reading on an ADC
 *  \details Take a median reading from an array of ADC readings.
 *      The number of samples is determined by the parameter "samples". If the
 *      "samples" parameter exceeds the maximum permissible number of samples (63), then only
 *      63 samples are taken. The number of samples taken, and the oversampled value are 
 *      written to the debug stream. 
 *  @param channel
 *  @param samples
 *  @return adc Value
 */

uint16_t adc::read_median (uint8_t channel, uint8_t num_samples)
{
	uint16_t samples[MAX_SAMPLES] ;
	//uint16_t sorted[MAX_SAMPLES];
	
	if (num_samples > MAX_SAMPLES ) num_samples=MAX_SAMPLES ;
	
	uint8_t i = 0;
	
	//throw away first value
	read_once(channel);
	
	for(i = 0; i < num_samples ;i++)
	{
	    samples[i] = read_once(channel);
	}
	
	qsort( samples, num_samples, sizeof(uint16_t), &compare );
	
	return samples[num_samples/2];
	
}


//-------------------------------------------------------------------------------------
/** \brief This overloaded operator "prints the A/D converter." 
 *  \details This operator allows the user to show current readings from all the A/D channels.
 *  @param serpt Reference to a serial port to which the printout  will be printed
 *  @param a2d Reference to the A/D driver which is being printed
 *  @return A reference to the same serial device on which we write information.
 *          This is used to string together things to write with "<<" operators
 */

emstream& operator << (emstream& serpt, adc& a2d)
{
	// The operator shows the readings on each ADC channel
	for(uint8_t i = 0; i<8; i++)
	{
	  serpt << a2d.read_once(i);
	}
	
	return (serpt);
}

