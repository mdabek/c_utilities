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
#include "circular_buffer.h"

//The following should be atomic 
uint16_t _CircularBuffer_NextPos(uint16_t pos, uint16_t bufferSize)
{
    if ((pos + 1) == bufferSize) {
        return 0;
    } else {
        return pos + 1;
    }
}
#define NEXT_HEAD(buf) _CircularBuffer_NextPos((buf)->Head, (buf)->Size)
#define NEXT_TAIL(buf) _CircularBuffer_NextPos((buf)->Tail, (buf)->Size)

uint32_t 
CircularBuffer_Init (PCIRCULAR_BUFFER buffer, PDATA_CONTAINER data, 
                    uint16_t elements_count, PFN_INSERT_ELEMENT element_insert)
{
    buffer->Buffer = data;
    buffer->Size = elements_count;
    buffer->Head = buffer->Tail = 0;
    buffer->ElementInsert = element_insert;

    return 1;
}

uint32_t 
CircularBuffer_IsEmpty (PCIRCULAR_BUFFER buffer)
{
    return buffer->Head == buffer->Tail;
}

uint32_t 
CircularBuffer_IsFull (PCIRCULAR_BUFFER buffer)
{
    return NEXT_HEAD(buffer) == buffer->Tail;
}

uint32_t 
CircularBuffer_Push (PCIRCULAR_BUFFER buffer, void* element)
{
    //Lock or make the following atomic
    //Note that the following check may be atomic on some platforms
    if (NEXT_HEAD(buffer) != buffer->Tail ) {
        buffer->ElementInsert(&(buffer->Buffer[buffer->Head]), element);
        buffer->Head = NEXT_HEAD(buffer);
        //unlock if locked
        return 1;
    }
    //unlock if locked
    return 0;
}

PDATA_CONTAINER 
CircularBuffer_Pop (PCIRCULAR_BUFFER buffer)
{
    PDATA_CONTAINER dc = NULL;

    if (CircularBuffer_IsEmpty(buffer)) {
       return dc;
    }
    
    //Lock or make the following atomic
    dc = &buffer->Buffer[buffer->Tail];
    buffer->Tail = NEXT_TAIL(buffer);
    //unlock if not atomic
    return dc;
}
