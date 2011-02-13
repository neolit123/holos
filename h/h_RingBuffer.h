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
#ifndef h_RingBuffer_included
#define h_RingBuffer_included
//----------------------------------------------------------------------
// modified (holosified) ringbuffer from jack (http://jackaudio.org/)
// see /extern/ringbuffer.h (and .c) for license info (GPL) and more..

// http://linux.about.com/library/cmd/blcmdl2_munlock.htm
//#ifdef USE_MLOCK
//#include <sys/mman.h>
//#endif /* USE_MLOCK */

struct h_RingBuffer_Ptrs
{
  char  *m_Buffer;
  size_t m_Length;
};

//----------------------------------------------------------------------

class h_RingBuffer
{

  private:
    char*           m_Buffer;
    size_t	        m_Size;
    size_t	        m_SizeMask;
    int		          m_Locked;
    volatile size_t m_WritePtr;
    volatile size_t m_ReadPtr;

  public:

    // Create a new ringbuffer to hold at least `sz' bytes of data.
    // The actual buffer size is rounded up to the next power of two.

    h_RingBuffer(size_t sz)
      {
        int power_of_two;
        for (power_of_two=1; 1<<power_of_two<(int)sz; power_of_two++);
        m_Size = 1 << power_of_two;
        m_SizeMask = m_Size;
        m_SizeMask -= 1;
        m_WritePtr = 0;
        m_ReadPtr = 0;
        m_Buffer = (char*)h_Malloc(m_Size);
        m_Locked = 0;
      }

    //----------

    ~h_RingBuffer()
      {
        if (m_Locked) unlock();
        free(m_Buffer);
        //free(rb);
      }

    //--------------------------------------------------

    /* Lock the data block of `rb' using the system call 'mlock'.  */

    int lock(void)
      {
        #ifdef USE_MLOCK
        if (mlock(m_Buffer,m_Size)) return -1;
        #endif
        m_Locked = 1;
        return 0;
      }

    void unlock(void)
      {
        #ifdef USE_MLOCK
        munlock(m_Buffer,m_Size);
        #endif
        m_Locked = 0;
      }

    // Reset the read and write pointers to zero.
    // This is not thread safe.

    void reset(void)
      {
        m_ReadPtr = 0;
        m_WritePtr = 0;
      }

    // Return the number of bytes available for reading.
    // This is the number of bytes in front of the read pointer
    // and behind the write pointer.

    size_t readSpace(void)
      {
        size_t w, r;
        w = m_WritePtr;
        r = m_ReadPtr;
        if (w>r) return w-r;
        else return (w-r+m_Size) & m_SizeMask;
      }

    // Return the number of bytes available for writing.
    // This is the number of bytes in front of the write pointer
    // and behind the read pointer.

    size_t writeSpace(void)
      {
        size_t w, r;
        w = m_WritePtr;
        r = m_ReadPtr;
        if (w>r) return ((r-w+m_Size)&m_SizeMask)-1;
        else if (w<r) return (r-w)-1;
        else return m_Size-1;
      }

    // The copying data reader.
    // Copy at most `cnt' bytes to `dest'.
    // Returns the actual number of bytes copied.

    size_t read(char* dest, size_t cnt)
      {
        size_t free_cnt;
        size_t cnt2;
        size_t to_read;
        size_t n1, n2;
        if ((free_cnt = readSpace())==0) return 0;
        to_read = cnt > free_cnt ? free_cnt : cnt;
        cnt2 = m_ReadPtr + to_read;
        if (cnt2 > m_Size)
        {
          n1 = m_Size - m_ReadPtr;
          n2 = cnt2 & m_SizeMask;
        }
        else
        {
          n1 = to_read;
          n2 = 0;
        }
        h_Memcpy(dest, &(m_Buffer[m_ReadPtr]), n1);
        m_ReadPtr = (m_ReadPtr + n1) & m_SizeMask;
        if (n2)
        {
          h_Memcpy(dest + n1, &(m_Buffer[m_ReadPtr]), n2);
          m_ReadPtr = (m_ReadPtr + n2) & m_SizeMask;
        }
        return to_read;
      }

    // The copying data reader w/o read pointer advance.
    // Copy at most `cnt' bytes to `dest'.
    // Returns the actual number of bytes copied.

    size_t peek(char *dest, size_t cnt)
      {
        size_t free_cnt;
        size_t cnt2;
        size_t to_read;
        size_t n1, n2;
        size_t tmp_ReadPtr;
        tmp_ReadPtr = m_ReadPtr;
        if ((free_cnt = readSpace()) == 0)
        {
          return 0;
        }
        to_read = cnt > free_cnt ? free_cnt : cnt;
        cnt2 = tmp_ReadPtr + to_read;
        if (cnt2 > m_Size)
        {
          n1 = m_Size - tmp_ReadPtr;
          n2 = cnt2 & m_SizeMask;
        }
        else
        {
          n1 = to_read;
          n2 = 0;
        }
        h_Memcpy(dest, &(m_Buffer[tmp_ReadPtr]), n1);
        tmp_ReadPtr = (tmp_ReadPtr + n1) & m_SizeMask;
        if (n2)
        {
          h_Memcpy(dest + n1, &(m_Buffer[tmp_ReadPtr]), n2);
        }
        return to_read;
      }

    // The copying data writer.
    // Copy at most `cnt' bytes to `rb'.
    // Returns the actual number of bytes copied.

    size_t write(const char *src, size_t cnt)
      {
        size_t free_cnt;
        size_t cnt2;
        size_t to_write;
        size_t n1, n2;
        if ((free_cnt = writeSpace()) == 0)
        {
          return 0;
        }
        to_write = cnt > free_cnt ? free_cnt : cnt;
        cnt2 = m_WritePtr + to_write;
        if (cnt2 > m_Size)
        {
          n1 = m_Size - m_WritePtr;
          n2 = cnt2 & m_SizeMask;
        }
        else
        {
          n1 = to_write;
          n2 = 0;
        }
        h_Memcpy(&(m_Buffer[m_WritePtr]), src, n1);
        m_WritePtr = (m_WritePtr + n1) & m_SizeMask;
        if (n2)
        {
          h_Memcpy(&(m_Buffer[m_WritePtr]), src + n1, n2);
          m_WritePtr = (m_WritePtr + n2) & m_SizeMask;
        }
        return to_write;
      }

    // Advance the read pointer `cnt' places.

    void readAdvance(size_t cnt)
      {
        size_t tmp = (m_ReadPtr + cnt) & m_SizeMask;
        m_ReadPtr = tmp;
      }

    // Advance the write pointer `cnt' places.

    void writeAdvance(size_t cnt)
      {
        size_t tmp = (m_WritePtr + cnt) & m_SizeMask;
        m_WritePtr = tmp;
      }

    // The non-copying data reader.
    // `vec' is an array of two places. Set the values at `vec' to hold the
    // current readable data.  If the readable data is in one segment the
    // second segment has zero length.

    void getReadVector(h_RingBuffer_Ptrs* vec)
      {
        size_t free_cnt;
        size_t cnt2;
        size_t w, r;
        w = m_WritePtr;
        r = m_ReadPtr;
        if (w > r) free_cnt = w-r;
        else free_cnt = (w-r+m_Size)&m_SizeMask;
        cnt2 = r + free_cnt;
        if (cnt2 > m_Size)
        {
          // Two part vector: the rest of the buffer after the current
          // write ptr, plus some from the start of the buffer.
          vec[0].m_Buffer = &(m_Buffer[r]);
          vec[0].m_Length = m_Size - r;
          vec[1].m_Buffer = m_Buffer;
          vec[1].m_Length = cnt2 & m_SizeMask;
        }
        else
        {
          // Single part vector: just the rest of the buffer
          vec[0].m_Buffer = &(m_Buffer[r]);
          vec[0].m_Length = free_cnt;
          vec[1].m_Length = 0;
        }
      }

    // The non-copying data writer.
    // `vec' is an array of two places. Set the values at `vec' to hold
    // the current writeable data. If the writeable data is in one segment
    // the second segment has zero length.

    void getWriteVector(h_RingBuffer_Ptrs* vec)
      {
        size_t free_cnt;
        size_t cnt2;
        size_t w, r;
        w = m_WritePtr;
        r = m_ReadPtr;
        if (w>r) free_cnt = ((r-w+m_Size)&m_SizeMask)-1;
        else if (w<r) free_cnt = (r-w)-1;
        else free_cnt = m_Size-1;
        cnt2 = w + free_cnt;
        if (cnt2>m_Size)
        {
          // Two part vector: the rest of the buffer after the current
          // write ptr, plus some from the start of the buffer.
          vec[0].m_Buffer = &(m_Buffer[w]);
          vec[0].m_Length = m_Size - w;
          vec[1].m_Buffer = m_Buffer;
          vec[1].m_Length = cnt2 & m_SizeMask;
        }
        else
        {
          vec[0].m_Buffer = &(m_Buffer[w]);
          vec[0].m_Length = free_cnt;
          vec[1].m_Length = 0;
        }
      }

};

//----------------------------------------------------------------------
#endif













