/*
  Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al

  This file is part of the Holos Library.
  http://holos.googlecode.com

  the Holos Library is free software: you can redistribute it and/or modify
  it under the terms of the Holos Library License, either version 1.0
  of the License, or (at your option) any later version.

  the Holos Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See LICENSE_HOLOS for more details.

  You should have received a copy of the Holos Library License
  If not, see <http://holos.googlecode.com/>.
*/
//----------------------------------------------------------------------
#ifndef h_Queue_included
#define h_Queue_included
//----------------------------------------------------------------------
/*

safe for single-reader, single-writer ??

(thread-safe?) circular/ring-buffer
based on code from jack.cpp (jacklab)

*/

#include "src/h_Stdlib.h"

//----------

template<class _T, int SIZE>
class h_Queue
{
  private:

    _T  m_Data[SIZE];
    volatile int m_WritePos;    // volatile?
    volatile int m_ReadPos;

  public:

    h_Queue()
      {
        h_Memset(m_Data,0,SIZE*(sizeof(_T)));
        m_WritePos = 0;
        m_ReadPos = 0;
      }

    //----------

    void write(_T a_Data)
      {
        m_Data[m_WritePos] = a_Data;
        m_WritePos = (m_WritePos + 1) % SIZE; // & (SIZE-1) if ^2
      }

    //----------

    // return num items copied into buffer
    int read(_T* a_Buffer)
      {
        int count = 0;
        int writepos = m_WritePos;
        if (m_ReadPos > writepos)
        {
          h_Memcpy( (char*)&a_Buffer[count],
                    (char*)&m_Data[m_ReadPos],
                    (SIZE - m_ReadPos) * sizeof(_T));
          count = SIZE - m_ReadPos;
          m_ReadPos = 0;
        }
        h_Memcpy( (char*)&a_Buffer[count],
                  (char*)&m_Data[m_ReadPos],
                  (writepos - m_ReadPos) * sizeof(_T));
        count += writepos - m_ReadPos;
        m_ReadPos = writepos;
        return count;
      }

    //TODO: return one:
    //_T* read(void)
    //  {
    //  }

};

//----------------------------------------------------------------------
#endif
