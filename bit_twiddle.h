/** \file bit_twiddle.h
 *    This file declares a function to set/set low a single bit
 *
 *  Revisions:
 *    \li 4-16-2014 Written by EB
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


#ifndef _BIT_TWIDDLE_H_
#define _BIT_TWIDDLE_H_

#include <avr/io.h>                         // Header for special function registers



void set_bit(volatile uint8_t& , uint8_t );
void set_bit(volatile uint8_t& , uint8_t , uint8_t );
void set_bit(volatile uint8_t& , uint8_t , uint8_t, uint8_t );
void set_bit(volatile uint8_t& , uint8_t , uint8_t, uint8_t, uint8_t );
void set_bit(volatile uint8_t& , uint8_t , uint8_t, uint8_t, uint8_t, uint8_t );
void set_bit(volatile uint8_t& , uint8_t , uint8_t, uint8_t, uint8_t, uint8_t, uint8_t );
void set_bit(volatile uint8_t& , uint8_t , uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t );
void set_bit(volatile uint8_t& , uint8_t , uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

void clear_bit(volatile uint8_t& , uint8_t );
void clear_bit(volatile uint8_t& , uint8_t , uint8_t );
void clear_bit(volatile uint8_t& , uint8_t , uint8_t, uint8_t );
void clear_bit(volatile uint8_t& , uint8_t , uint8_t, uint8_t, uint8_t );
void clear_bit(volatile uint8_t& , uint8_t , uint8_t, uint8_t, uint8_t, uint8_t );
void clear_bit(volatile uint8_t& , uint8_t , uint8_t, uint8_t, uint8_t, uint8_t, uint8_t );
void clear_bit(volatile uint8_t& , uint8_t , uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t );
void clear_bit(volatile uint8_t& , uint8_t , uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

void clear_and_set( volatile uint8_t&, uint8_t );



#endif