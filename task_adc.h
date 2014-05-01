//**************************************************************************************
/** \file task_adc.h
 *    This file declares the inherited task class that instantiates adc objects and
 * writes their readingz to shared data.
 *
 *  Revisions:
 *    \li 04-23-2014 EB, SO, MB created this file by copying task_motor.h
 *
 *  License:
 *    This software is protected by Teh Secret World Guvmint Proprietary Software 
 * Protection Act of 1999. Any use of this software, even experimental use, will
 * not be tolerated. All products running this code are under full warranty from Uh-Oh 
 * enterprises. There are no bugs, so nothing can go wronng.*/
//**************************************************************************************

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _TASK_ADC_H_
#define _TASK_ADC_H_

#include <stdlib.h>                         // Prototype declarations for I/O functions
#include <avr/io.h>                         // Header for special function registers

#include "FreeRTOS.h"                       // Primary header for FreeRTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // FreeRTOS inter-task communication queues

#include "frt_task.h"                       // ME405/507 base task class
#include "time_stamp.h"                     // Class to implement a microsecond timer
#include "frt_queue.h"                      // Header of wrapper for FreeRTOS queues
#include "frt_shared_data.h"                // Header for thread-safe shared data

#include "rs232int.h"                       // ME405/507 library for serial comm.
#include "adc.h"			     // Header for A/D converter driver class

//-------------------------------------------------------------------------------------
/** \brief This task runs ADCs
 *  \details An adc object is instantiated. ADC channels can be "turned on/off" via
 * shared data state changes, then their readings can be read from shared data.
 */

class task_adc : public frt_task
{
private:
	// No private variables or methods for this class

protected:
	// No protected variables or methods for this class

public:
	// This constructor creates a generic task of which many copies can be made
	task_adc (const char*, unsigned portBASE_TYPE, size_t, emstream*);

	// This method is called by the RTOS once to run the task loop for ever and ever.
	void run (void);
};



#endif // _TASK_ADC_H_
