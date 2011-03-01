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
#ifndef h_Array_included
#define h_Array_included
//----------------------------------------------------------------------

#include "src/h_Memory.h"

#define SIZE_INIT 16
#define SIZE_MULT 2

#define array_malloc  malloc
#define array_calloc  calloc
#define array_realloc realloc
#define array_free    free

//#define array_malloc  h_Malloc
//#define array_calloc  h_Calloc
//#define array_realloc h_Realloc
//#define array_free    h_Free

template<class _T>
class h_Array
{
	private:

    _T*          m_Buffer;
    unsigned int m_Tsize;
    unsigned int m_Size;
    unsigned int m_RealSize;

  public:

		h_Array()
			{
		  	m_Tsize = sizeof(_T);
        m_RealSize = SIZE_INIT;
			  m_Size = 0;
			  m_Buffer = (_T*)array_malloc(m_RealSize*m_Tsize);
			}

    h_Array(const h_Array& a_Array)
      {
        m_Tsize = sizeof(_T);
        m_Buffer = (_T*)array_malloc(m_Tsize*a_Array.m_RealSize);
        h_Memcpy(m_Buffer, a_Array.m_Buffer, m_Tsize*a_Array.m_RealSize);
        m_RealSize = a_Array.m_RealSize;
        m_Size = a_Array.m_Size;
      }

    ~h_Array()
      {
        if (m_Buffer)
        {
          array_free(m_Buffer);
          m_Buffer = NULL;
        }
      }

    _T& operator [] (const unsigned int a_Index)
      {
        return m_Buffer[a_Index];
      }

    _T& item(const unsigned int a_Index)
      {
        return m_Buffer[a_Index];
      }

    int size()
      {
        return m_Size;
      }

    void* ptr()
      {
        return m_Buffer;
      }

    h_Array& operator = (const h_Array &a_Array)
      {
        if (this==&a_Array)
          return *this;
        if (a_Array.m_Size==0)
          clear();
        setSize(a_Array.m_Size);
        h_Memcpy(m_Buffer, a_Array.m_Buffer, m_Tsize*a_Array.m_Size);
        return *this;
      }

    void clear(bool a_Erase=true)
      {
        m_Size = 0;
        if (a_Erase)
        {
          m_Buffer = (_T*)array_realloc((char*)m_Buffer, m_Tsize*SIZE_INIT);
          m_RealSize = SIZE_INIT;
        }
      }

    void append(const _T& a_Item)
      {
        m_Size++;
        if (m_Size > m_RealSize)
        {
          m_RealSize *= SIZE_MULT;
          m_Buffer = (_T*)array_realloc(m_Buffer, m_Tsize*m_RealSize);
        }
        m_Buffer[m_Size-1] = a_Item;
      }

    void setSize(const unsigned int a_Size)
      {
        if (a_Size != 0)
        {
          if ( (a_Size>m_RealSize) || (a_Size<m_RealSize/SIZE_MULT) )
          {
            m_RealSize = a_Size;
            m_Buffer = (_T*)array_realloc(m_Buffer, m_Tsize*m_Size);
            m_Size = a_Size;
          }
        }
        else clear();
      }

    void remove(const unsigned int a_Pos)
      {
        if (m_Size==1) clear();
        else
        {
          for(unsigned int i=a_Pos; i<m_Size-1; i++ )
            m_Buffer[i] = m_Buffer[i+1];
          m_Size--;
        }
      }

    //TODO: insert

    void push(_T a_Item)
      {
        append(a_Item);
      }

    _T pop(void)
      {
        if (m_Size>0)
        {
          m_Size--;
          return m_Buffer[m_Size];
        }
        else return NULL;
      }
};

#undef array_malloc
#undef array_calloc
#undef array_realloc
#undef array_free

//----------------------------------------------------------------------
#endif
