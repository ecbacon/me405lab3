/** \file bit_twiddle.cpp
 *    This file contains a function to set / set low a bit
 *
 *  Revisions:
 *    \li 4-16-2014 written by EB
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
//======================================================================================



#include "bit_twiddle.h"

/** \brief This function sets/unsets bits
 *  \details set the specified register's specified bit to the specified value
 *  
 *  @param reg A register to change
 *  @param bit which bit in register to change
 *  @param value what value to change it to (0 or high)
 */

void set_bit(volatile uint8_t& reg, uint8_t bit)
{

    reg |= (1<<bit);
    
  return;
}

void set_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2)
{
	reg |= (1<<bit1)|(1<<bit2);
	return;
}

void set_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2, uint8_t bit3)
{
	reg |= (1<<bit1)|(1<<bit2)|(1<<bit3);
	return;
}

void set_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t bit4)
{
	reg |= (1<<bit1)|(1<<bit2)|(1<<bit3)|(1<<bit4);
	return;
}


void set_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t bit4, uint8_t bit5)
{
	reg |= (1<<bit1)|(1<<bit2)|(1<<bit3)|(1<<bit4)|(1<<bit5);
	return;
}


void set_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t bit4, uint8_t bit5, uint8_t bit6)
{
	reg |= (1<<bit1)|(1<<bit2)|(1<<bit3)|(1<<bit4)|(1<<bit5)|(1<<bit6);
	return;
}


void set_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t bit4, uint8_t bit5, uint8_t bit6, uint8_t bit7)
{
	reg |= (1<<bit1)|(1<<bit2)|(1<<bit3)|(1<<bit4)|(1<<bit5)|(1<<bit6)|(1<<bit7);
	return;
}

void set_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t bit4, uint8_t bit5, uint8_t bit6, uint8_t bit7, uint8_t bit8)
{
	reg |= (1<<bit1)|(1<<bit2)|(1<<bit3)|(1<<bit4)|(1<<bit5)|(1<<bit6)|(1<<bit7)|(1<<bit8);
	return;
}

void clear_bit(volatile uint8_t& reg, uint8_t bit)
{
	
	reg &= ~(1<<bit);
	
	return;
}

void clear_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2)
{
	reg &= ~( (1<<bit1)|(1<<bit2) );
	return;
}

void clear_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2, uint8_t bit3)
{
	reg &= ~((1<<bit1)|(1<<bit2)|(1<<bit3) );
	return;
}

void clear_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t bit4)
{
	reg &= ~(( 1<<bit1)|(1<<bit2)|( 1<<bit3)|( 1<<bit4));
	return;
}


void clear_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t bit4, uint8_t bit5)
{
	reg &= ~(( 1<<bit1)|( 1<<bit2)|( 1<<bit3)|( 1<<bit4)|( 1<<bit5));
	return;
}


void clear_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t bit4, uint8_t bit5, uint8_t bit6)
{
	reg &= ~(( 1<<bit1)|( 1<<bit2)|( 1<<bit3)|( 1<<bit4)|( 1<<bit5)|( 1<<bit6));
	return;
}


void clear_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t bit4, uint8_t bit5, uint8_t bit6, uint8_t bit7)
{
	reg &= ~(( 1<<bit1)|( 1<<bit2)|( 1<<bit3)|( 1<<bit4)|( 1<<bit5)|( 1<<bit6)|( 1<<bit7));
	return;
}

void clear_bit(volatile uint8_t& reg, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t bit4, uint8_t bit5, uint8_t bit6, uint8_t bit7, uint8_t bit8)
{
	reg &= ~(( 1<<bit1)|( 1<<bit2)|( 1<<bit3)|( 1<<bit4)|( 1<<bit5)|( 1<<bit6)|( 1<<bit7)|( 1<<bit8));
	return;
}


/**
 * \brief This function clears a reg and sets one bit high
 * \details set all bits low, then set specified bit high
 * 
 * @param reg the register to change
 * @param bit the bit to set high
 */
void clear_and_set( volatile uint8_t& reg, uint8_t bit)
{
  reg = 0;
  reg |= (1<<bit);
  return;
}