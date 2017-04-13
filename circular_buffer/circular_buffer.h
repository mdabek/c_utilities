/**
     Copyright (c) 2017, Marek Dabek
     All rights reserved.

     Redistribution and use in source and binary forms, with or without
     modification, are permitted provided that the following conditions are met:

     1. Redistributions of source code must retain the above copyright notice, this
        list of conditions and the following disclaimer.
     2. Redistributions in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.

     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
      LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
     ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     The views and conclusions contained in the software and documentation are those
     of the authors and should not be interpreted as representing official policies,
     either expressed or implied, of the FreeBSD Project.
*/

//Requires: <stdint.h>
#include <stdio.h>
#include <stdint.h>

#ifndef _CIRCULAR_BUFFER_
#define _CIRCULAR_BUFFER_
/**
 * DATA_CONTAINER is a base type, which needs to be used as a first field of the 
 * data type stored by the CIRCULAR_BUFFER.
 * E.g.:
 * struct _MY_DATA {
 *      DATA_CONTAINER ;
 *      SOME_DATA*  Data;
 *      Uint8_t     IsDataValid;
 * };
*/
typedef struct _DATA_CONTAINER {
    uint16_t    DataSize;
} DATA_CONTAINER, *PDATA_CONTAINER;


/*
 * The function pointer which adds an element to the circular buffer data 
 * container queue
 */
typedef uint8_t (*PFN_INSERT_ELEMENT)(PDATA_CONTAINER, void*);

typedef struct _CIRCULAR_BUFFER {
    PDATA_CONTAINER  Buffer;     //Array of DATA_CONTAINERs

    uint16_t         Size;       //Number of elements that the buffer can hold
    uint16_t         Head;       //Head of the buffer - the last added element 
    uint16_t         Tail;       //Tail of the buffer - the first added element

    PFN_INSERT_ELEMENT   ElementInsert; 
} CIRCULAR_BUFFER, *PCIRCULAR_BUFFER;

/**
 * Initializes the buffer structure - do not initialize by hand, or else the 
 * compatibility may be broken!
 * */
uint32_t 
CircularBuffer_Init (PCIRCULAR_BUFFER buffer, PDATA_CONTAINER data, 
                    uint16_t elements_count, PFN_INSERT_ELEMENT element_insert);
/**
 * Checks if the circular buffer is empty. 
 * Returns: TRUE/FALSE
 */
uint32_t CircularBuffer_IsEmpty (PCIRCULAR_BUFFER );
/**
 * Checks if the circular buffer is full
 * Returns: TRUE/FALSE
 */
uint32_t CircularBuffer_IsFull  (PCIRCULAR_BUFFER );

/**
 * Adds an element to the buffer.
 * Returns: success or error code
 */
uint32_t CircularBuffer_Push    (PCIRCULAR_BUFFER, void*);

/**
 * Adds an element to the buffer.
 * Returns: valid pointer to the data container or NULL
 */
PDATA_CONTAINER CircularBuffer_Pop     (PCIRCULAR_BUFFER );

#endif //_CIRCULAR_BUFFER_
