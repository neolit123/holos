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

#ifndef h_Cpu_included
#define h_Cpu_included
//----------------------------------------------------------------------

#include "src/h_Defines.h"
#include "src/h_Stdlib.h"

//----------------------------------------------------------------------

// http://bits.stephan-brumme.com/endianess.html
bool h_IsLittleEndian(void)
  {
    short pattern = 0x0001;
    return *(char*)&pattern == 0x01;
  }

//----------

#ifndef h_BitGet
  #define h_BitGet(x, bit)  ( 1  &  ((x)  >> (bit)))
#endif

// fPIC compatible
#define _H_CPU_EBX_REG "r"
#define _H_CPU_EBX_STORE  \
  "pushl %%ebx;"          \
  "cpuid;"                \
  "movl %%ebx, %1;"       \
  "popl %%ebx;"

class h_Cpu
{
  private:
    unsigned char isCalled;
    char          cpustringbuf[H_MAX_STRINGSIZE];

  public:
    unsigned int  caps;
    unsigned char _SSE3,
                  _SSSE3,
                  _FPU,
                  _CMOV,
                  _SSE,
                  _SSE2,
                  _SSE4A,
                  _SSE5,
                  _MMX,
                  _MMXEXT,
                  _3DNOW,
                  _3DNOWEXT;

    h_Cpu(void)   { isCalled=0; caps=0; }
    ~h_Cpu(void)  {}

    // get cpu specifiction
    void
    CpuId(const int fcall=33139, int* eax=NULL, int* ebx=NULL, int* ecx=NULL, int* edx=NULL)
    {
      if (fcall == 33139)  // 33139 = some default number
      {
        isCalled = 1;
        int a, b, c, d;
        // -----------------
        // 0x00000001
        __asmv
        (
          _H_CPU_EBX_STORE
          : "=a" (a), "="_H_CPU_EBX_REG"" (b),
            "=c" (c), "=d" (d) : "a" (0x00000001) : "cc"
        );
        _SSE3   = h_BitGet(c, 0);
        _SSSE3  = h_BitGet(c, 9);
        _FPU    = h_BitGet(d, 0);
        _CMOV   = h_BitGet(d, 15);
        _SSE    = h_BitGet(d, 25);
        _SSE2   = h_BitGet(d, 26);
        // -----------------
        // 0x80000001
        __asmv
        (
          _H_CPU_EBX_STORE
          : "=a" (a), "="_H_CPU_EBX_REG"" (b),
            "=c" (c), "=d" (d) : "a" (0x80000001) : "cc"
        );
        _SSE4A    = h_BitGet(c, 4);
        _SSE5     = h_BitGet(c, 11);
        _MMX      = h_BitGet(d, 23);
        _MMXEXT   = h_BitGet(d, 22);
        _3DNOW    = h_BitGet(d, 31);
        _3DNOWEXT = h_BitGet(d, 30);
      }
      // user defined call
      // -----------------
      else
        __asmv
        (
          _H_CPU_EBX_STORE
          : "=a" (*eax), "="_H_CPU_EBX_REG"" (*ebx),
            "=c" (*ecx), "=d" (*edx) : "a" (fcall) : "cc"
        );
    }

    // get cpu caps

    unsigned int
    CpuCaps(void)
    {
      if (!isCalled) CpuId();
      if (_SSE3)      caps |= 0x0001;
      if (_SSSE3)     caps |= 0x0002;
      if (_FPU)       caps |= 0x0004;
      if (_CMOV)      caps |= 0x0008;
      if (_SSE)       caps |= 0x0010;
      if (_SSE2)      caps |= 0x0020;
      if (_SSE4A)     caps |= 0x0040;
      if (_SSE5)      caps |= 0x0080;
      if (_MMX)       caps |= 0x0100;
      if (_MMXEXT)    caps |= 0x0200;
      if (_3DNOW)     caps |= 0x0400;
      if (_3DNOWEXT)  caps |= 0x0800;
      return caps;
    }

    // h_CpuCapsString
    char*
    CpuCapsString(void)
    {
      if (!isCalled) CpuId();
      cpustringbuf[0] = 0;
      if (_SSE3)     h_Strcat(cpustringbuf,(char*)"sse3 "); // space at the end make them easier to string together
      if (_SSSE3)    h_Strcat(cpustringbuf,(char*)"ssse3 ");
      if (_FPU)      h_Strcat(cpustringbuf,(char*)"fpu ");
      if (_CMOV)     h_Strcat(cpustringbuf,(char*)"cmov ");
      if (_SSE)      h_Strcat(cpustringbuf,(char*)"sse ");
      if (_SSE2)     h_Strcat(cpustringbuf,(char*)"sse2 ");
      if (_SSE4A)    h_Strcat(cpustringbuf,(char*)"sse4a ");
      if (_SSE5)     h_Strcat(cpustringbuf,(char*)"sse5 ");
      if (_MMX)      h_Strcat(cpustringbuf,(char*)"mmx ");
      if (_MMXEXT)   h_Strcat(cpustringbuf,(char*)"mmxext ");
      if (_3DNOW)    h_Strcat(cpustringbuf,(char*)"3dnow ");
      if (_3DNOWEXT) h_Strcat(cpustringbuf,(char*)"3dnowext ");
      return cpustringbuf;
    }

    // call rdtsc()
    #ifdef _H_64BIT_
      inline unsigned long rdtsc(void)
      {
        /*
        unsigned int low, high;
        __asmv ( "rdtsc;" : "=a" (low), "=d" (high) );
        return ( (low) | ( (unsigned long)(high) << 32 ) );
        */
        unsigned long val;
        __asmv ( "rdtsc;" : "=A" (val) );
        return val;
        // ^ check if problems occur with 64bit return
      }
    #endif
    #ifdef _H_32BIT_
      inline unsigned long rdtsc(void)
      {
        unsigned long long val;
        __asmv
        (
          // ## serialization causes delayed segmentation faults !
          //"cpuid;"
          "rdtsc;"
          //"leal %0, %%ecx;"
          //"movl %%eax, (%%ecx);"
          //"movl %%edx, 4(%%ecx);"
          : "=A" (val)
          //:: "%eax", "%ebx", "%ecx", "%edx"
        );
        return (unsigned long)val;
      }
    #endif

};

//----------------------------------------------------------------------
#endif
