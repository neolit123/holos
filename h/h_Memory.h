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
#ifndef h_Memory_included
#define h_Memory_included
//----------------------------------------------------------------------

#include <stdlib.h>

#define h_Malloc  malloc
#define h_Calloc  calloc
#define h_Realloc realloc
#define h_Free    free

//----------------------------------------------------------------------
#endif

























































//
//
//
//
///*
// * This file is part of Axonlib.
// *
// * Axonlib is free software: you can redistribute it and/or modify
// * it under the terms of the Axonlib License, either version 1.0
// * of the License, or (at your option) any later version.
// *
// * Axonlib is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// * See LICENSE_AX for more details.
// *
// * You should have received a copy of the Axonlib License
// * If not, see <http://axonlib.googlecode.com/>.
// */
//
///*
//  axMalloc.h
//
//  ==============================================================================
//  Axonlib's own memory allocation routines:
//  axMalloc, axCalloc, axRealloc, axFree
//  ==============================================================================
//  based on code by:
//    DJ Delorie (DJGPP)
//      http://www.delorie.com/djgpp/malloc/
//    where licensing permits use in the case of Axonlib:
//      http://www.delorie.com/djgpp/dl/ofc/simtel/v2/copying.dj
//
//  and the following article by Joerg Walter, Doug Lea et al (available in the
//  public domain):
//    http://www.genesys-e.de/jwalter/mix4win.htm
//
//  original file header by DJ Dolorie from here bellow:
//  ==============================================================================
//*/
//
///* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
//
//// end of original header
//// =============================================================================
//
//
///*
//NOTES:
//  - the custom memory allocator is currenly disabled due to a conflict with
//    picopng.h ( using stdlib malloc() )
//
//TODO:
//  - resolve the picopng conflict
//  - better error handling
//  - fragmentation level tests
//    ============================================================================
//*/
//
//#ifndef axMalloc_included
//#define axMalloc_included
//
//#ifdef AX_HOT_INLINE_MALLOC
//  #define __axmalloc_inline __hotinline
//#else
//  #define __axmalloc_inline inline
//#endif
//
//#ifndef AX_USE_MALLOC
//  #define AX_NO_MALLOC  // disable the local malloc by default
//#endif
//
//#ifdef AX_NO_MALLOC     // no axMalloc - use stdlib.h malloc
//  #include "stdlib.h"
//  #define axMalloc    malloc
//  #define axCalloc    calloc
//  #define axRealloc   realloc
//  #define axFree      free
//
//  // no debug for these
//  #define _axMalloc   malloc
//  #define _axCalloc   calloc
//  #define _axRealloc  realloc
//  #define _axFree     free
//
//#else // use axMalloc
//
///*
//  from here bellow adapted version of Joerg Walter's MMAP emulation
//  for windows with mutex.
//*/
//#ifdef WIN32
//  #include "windows.h"
//
//  static __thread long g_sl;
//
//  /*
//  // inline these two
//  __axmalloc_inline void slwait (long* sl)
//    while ( InterlockedCompareExchange( sl, 1, 0) != 0 )
//      Sleep(0);
//   __axmalloc_inline void slrelease (long* sl)
//    InterlockedExchange( sl, 0 );
//  */
//
//  __axmalloc_inline long getpagesize (void)
//  {
//    static long g_pagesize = 0;
//    if (! g_pagesize)
//    {
//      SYSTEM_INFO system_info;
//      GetSystemInfo (&system_info);
//      g_pagesize = system_info.dwPageSize;
//    }
//    return g_pagesize;
//  }
//
//  __axmalloc_inline long getregionsize (void)
//  {
//    static long g_regionsize = 0;
//    if (! g_regionsize)
//    {
//      SYSTEM_INFO system_info;
//      GetSystemInfo (&system_info);
//      g_regionsize = system_info.dwAllocationGranularity;
//    }
//    return g_regionsize;
//  }
//
//  __axmalloc_inline void* axMmap (long size)
//  {
//    register void* ptr = NULL;
//    static long g_pagesize;
//    static long g_regionsize;
//    //slwait(&g_sl);
//    while ( InterlockedCompareExchange( &g_sl, 1, 0) != 0 )
//      Sleep(0);
//    if (!g_pagesize)
//      g_pagesize = getpagesize();
//    if (!g_regionsize)
//      g_regionsize = getregionsize();
//    ptr = VirtualAlloc(ptr, size,
//      MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
//    //slrelease (&g_sl);
//    InterlockedExchange( &g_sl, 0 );
//    return ptr;
//  }
//
//  __axmalloc_inline long axMunmap (void *ptr,
//    __attribute__((unused)) long size=0)
//  {
//    static long g_pagesize;
//    static long g_regionsize;
//    int rc = NULL;
//    //slwait(&g_sl);
//    while ( InterlockedCompareExchange( &g_sl, 1, 0) != 0 )
//      Sleep(0);
//    if (!g_pagesize)
//      g_pagesize = getpagesize ();
//    if (!g_regionsize)
//      g_regionsize = getregionsize ();
//    if (!VirtualFree (ptr, 0, MEM_RELEASE) )
//      rc = 0;
//    //slrelease (&g_sl);
//    InterlockedExchange( &g_sl, 0 );
//    return rc;
//  }
//
//#endif
//
//#ifdef linux
//  #include "unistd.h" // sbrk()
//#endif
//
///**
//  from here bellow optimized and modified version of DJ Delorie's malloc
//  routines (malloc1.c)
//*/
//
//#define AX_M_MAX_BUCKETS 32
//
//unsigned char* buckets[AX_M_MAX_BUCKETS] = {0};
//unsigned int bucket2size[AX_M_MAX_BUCKETS] = {0};
//
//static __axmalloc_inline unsigned int size2bucket(unsigned size)
//{
//  int rv = 0x1f;
//  int bit = ~0x10;
//  if (size < 4) size = 4;
//  size = (size + 3) & ~3;
//  unsigned int i = 0;
//  while (i < 5)
//  {
//    if ((rv & bit) < AX_M_MAX_BUCKETS)
//      if (bucket2size[rv & bit] >= size)
//        rv &= bit;
//    bit >>= 1;
//    i++;
//  }
//  return rv;
//}
//
//static __axmalloc_inline void init_buckets()
//{
//  register unsigned int b = 0;
//  while (b < AX_M_MAX_BUCKETS)
//  {
//    bucket2size[b] = (1 << b);
//    b++;
//  }
//}
//
//// axMalloc
//static __axmalloc_inline void* axMalloc (register unsigned int size)
//{
//  __builtin_printf("%d\n", size);
//  if (size <= 0)
//    return NULL;
//  register unsigned char* rv;
//  register unsigned int b;
//  if (!bucket2size[0])
//    init_buckets();
//  b = size2bucket(size);
//  if (buckets[b])
//  {
//    rv = buckets[b];
//    buckets[b] = *(unsigned char**)rv; // !!
//    return rv;
//  }
//  size = bucket2size[b] + 4;
//  // os specific calls
//  #ifdef linux
//    //rv = (char*)mmap(rv, size, ...);  // #include "sys/mman.h"
//    rv = (unsigned char*)sbrk(size);    // sbrk = legacy
//  #endif
//  #ifdef WIN32
//    rv = (unsigned char*)axMmap(size);
//  #endif
//  *(unsigned int*)rv = b;
//  rv += 4;
//  return (void*)rv;
//}
//
//// axCalloc
//static __axmalloc_inline void* axCalloc (register const unsigned int n,
//  register unsigned int size)
//{
//  //__builtin_printf("<< axCalloc\n");
//  size *= n;
//  if (size <= 0)
//    return NULL;
//  else
//  {
//    void* ptr = axMalloc(size);
//    register char* cptr = (char*) ptr;
//    while (size--)
//      *cptr++ = 0;
//    return ptr;
//  }
//}
//
//
//// axFree
//static __axmalloc_inline void axFree (void* _ptr)
//{
//  //__builtin_printf("<< axFree\n");
//  if (_ptr)
//  {
//    register unsigned char* ptr = (unsigned char*)_ptr;
//    register unsigned int b = *(unsigned int*)(ptr-4);
//    if (b < AX_M_MAX_BUCKETS)
//    {
//      *(unsigned char**)ptr = buckets[b];
//      buckets[b] = ptr;
//    }
//    // compare and free
//    axMunmap(_ptr);
//  }
//}
//
//// axRealloc
//static __axmalloc_inline void* axRealloc (void* _ptr,
//  register const unsigned int size)
//{
//  //__builtin_printf("<< axRealloc\n");
//  // case null pointer
//  if (_ptr == NULL)
//    return axMalloc(size);
//  else
//  {
//    if (size == 0)
//      axFree(_ptr);
//    else
//    {
//      register char* newptr;
//      register char* ptr = (char*)_ptr;
//      unsigned int oldsize = bucket2size[*(unsigned int*)(ptr-4)];
//      if (size <= oldsize)
//        return ptr;
//      newptr = (char*) axMalloc(size * sizeof(_ptr));
//      // -- memcpy
//      while (oldsize--)
//        *ptr++ = *newptr++;
//      // --
//      axFree(ptr);
//      return (void*)newptr;
//    }
//  }
//  return _ptr;
//}
//
//#define _axMalloc  axMalloc
//#define _axCalloc  axCalloc
//#define _axFree    axFree
//#define _axRealloc axRealloc
//
//#endif // !AX_NO_MALLOC
//
//// -----------------------------------------------------------------------------
//// enable local debug
//// -----------------------------------------------------------------------------
//#if defined (AX_DEBUG) && defined (AX_DEBUG_MEM)
//  #include "axDebug.h"
//  #include "axUtils.h"
//
//  #ifdef AX_NO_MALLOC
//    #include "malloc.h"
//  #endif
//
//  #ifdef AX_WIN32
//    #define malloc_usable_size _msize
//  #endif
//
//  // global data, can be 'dangerous' for multiple instances
//  static __thread int _axMemTotal = 0;
//
//  // malloc debug
//  __axmalloc_inline void* axMallocDebug
//    (register unsigned int _size, const char* _file, const unsigned int _line,
//    const unsigned int flag = 0)
//  {
//    char _name[20];
//    #ifdef AX_NO_MALLOC
//      void* _ptr = malloc(_size);
//      _axMemTotal += malloc_usable_size(_ptr);
//      if (flag)
//        axStrcpy(_name, "malloc(new), ");
//      else
//        axStrcpy(_name, "malloc, ");
//    #else
//      void* _ptr = axMalloc(_size);
//      _axMemTotal += bucket2size[*(unsigned int*)((char*)_ptr-4)];
//      if (flag)
//        axStrcpy(_name, "axMalloc(new), ");
//      else
//        axStrcpy(_name, "axMalloc, ");
//    #endif
//    // output cout / log
//    _trace
//    (
//      "[" << axGetFileName(_file) << "|" << _line << "] " << _name <<
//      (void*)&_ptr << ", " << _size << ", " << _axMemTotal
//    );
//    return _ptr;
//  }
//
//  // calloc debug
//  __axmalloc_inline void* axCallocDebug
//    (register const unsigned int _n, register unsigned int _size,
//    const char* _file, const unsigned int _line)
//  {
//    char _name[20];
//    #ifdef AX_NO_MALLOC
//      void* _ptr = calloc(_n, _size);
//      unsigned int size = malloc_usable_size(_ptr);
//      _axMemTotal += size;
//      axStrcpy(_name, "calloc, ");
//    #else
//      void* _ptr = axCalloc(_n, _size);
//      unsigned int size = bucket2size[*(unsigned int*)((char*)_ptr-4)];
//      _axMemTotal += size;
//      axStrcpy(_name, "axCalloc, ");
//    #endif
//    // output cout / log
//    _trace
//    (
//      "[" << axGetFileName(_file) << "|" << _line << "] " << _name <<
//      (void*)&_ptr << ", " << _n << ", " << _size << ", " << size << ", " <<
//      _axMemTotal
//    );
//    return _ptr;
//  }
//
//  // realloc debug
//  __axmalloc_inline void* axReallocDebug
//    (void* _ptr, register const unsigned int _size, const char* _file,
//    const unsigned int _line)
//  {
//    char _name[20];
//    #ifdef AX_NO_MALLOC
//      int nsize = malloc_usable_size(_ptr);
//      if (_axMemTotal - nsize >= 0)
//        _axMemTotal -= nsize;
//      else
//        _axMemTotal = 0;
//      void* _ptr0 = realloc(_ptr, _size);
//      _axMemTotal += malloc_usable_size(_ptr0);
//      axStrcpy(_name, "realloc, ");
//    #else
//      int nsize = bucket2size[*(unsigned int*)((char*)_ptr-4)];
//      if (_axMemTotal - nsize >= 0)
//        _axMemTotal -= nsize;
//      void* _ptr0 = axRealloc(_ptr, _size);
//      _axMemTotal += bucket2size[*(unsigned int*)((char*)_ptr0-4)];;
//      axStrcpy(_name, "axRealloc, ");
//    #endif
//    // output cout / log
//    _trace
//    (
//      "[" << axGetFileName(_file) << "|" << _line << "] " << _name <<
//      (void*)&_ptr << ", " << (void*)&_ptr0 <<
//      ", " << _size << ", " << _axMemTotal
//    );
//
//    return _ptr0;
//  }
//
//  // free debug
//  __axmalloc_inline void axFreeDebug
//    (void* _ptr, const char* _file, const unsigned int _line,
//    const unsigned int flag = 0)
//  {
//    if (_axMemTotal && _ptr)
//    {
//      int _size = 0;
//      char _name[20];
//      #ifdef AX_NO_MALLOC
//        _size = malloc_usable_size(_ptr);
//        if (flag)
//          axStrcpy(_name, "free(delete), ");
//        else
//          axStrcpy(_name, "free, ");
//      #else
//        unsigned int b = *(unsigned int*)((char*)_ptr-4);
//        if (b < AX_M_MAX_BUCKETS)
//          _size = bucket2size[b];
//        if (flag)
//          axStrcpy(_name, "axFree(delete), ");
//        else
//          axStrcpy(_name, "axFree, ");
//      #endif
//      // output cout / log
//      if (_axMemTotal - _size >= 0)
//      {
//        _axMemTotal -= _size;
//        _trace
//        (
//          "[" << axGetFileName(_file) << "|" << _line << "] " << _name <<
//          (void*)&_ptr << ", " << _size << ", " << _axMemTotal
//        );
//
//        #ifdef AX_NO_MALLOC
//          free(_ptr);
//        #else
//          axFree(_ptr);
//        #endif
//      }
//      else
//        _axMemTotal = 0;
//    }
//  }
//
//  // no debug for these: _ax*
//  #undef _axMalloc
//  #undef _axCalloc
//  #undef _axRealloc
//  #undef _axFree
//  __axmalloc_inline void* _axMalloc (register const unsigned int size)
//    { return axMalloc(size); }
//  __axmalloc_inline void* _axCalloc (register const unsigned int n,
//    register unsigned int size)  { return axCalloc(n, size); }
//  __axmalloc_inline void* _axRealloc (register void* _ptr,
//    register const unsigned int size) { return axRealloc(_ptr, size); }
//  __axmalloc_inline void  _axFree (register void* _ptr)
//    { axFree(_ptr); }
//
//  // clear previous definitions (if any)
//  #ifdef AX_NO_MALLOC
//    #undef axMalloc
//    #undef axCalloc
//    #undef axRealloc
//    #undef axFree
//  #endif
//
//  // macro overrides here
//  #define axMalloc(s)     axMallocDebug   (s, __FILE__, __LINE__)
//  #define axCalloc(n, s)  axCallocDebug   (n, s, __FILE__, __LINE__)
//  #define axRealloc(p, s) axReallocDebug  (p, s, __FILE__, __LINE__)
//  #define axFree(p)       axFreeDebug     (p, __FILE__, __LINE__)
//
//  // same for the standard methods if 'AX_NO_MALLOC'
//  #ifdef AX_NO_MALLOC
//    #define malloc(s)       axMallocDebug   (s, __FILE__, __LINE__)
//    #define calloc(n, s)    axCallocDebug   (n, s, __FILE__, __LINE__)
//    #define realloc(p, s)   axReallocDebug  (p, s, __FILE__, __LINE__)
//    #define free(p)         axFreeDebug     (p, __FILE__, __LINE__)
//  #endif
//
//  #ifdef AX_DEBUG_NEW
//    // define some helpers for the delete operator
//    static __thread char* ax_del_file;
//    static __thread unsigned int ax_del_line;
//    unsigned int axDebugSetDelete(const char* file, const unsigned int line)
//    {
//      ax_del_file = (char*)file;
//      ax_del_line = line;
//      return 1;
//    }
//
//    // overload operators new & delete with debug
//    __axmalloc_inline void* operator new (const size_t size,
//      const char* file, unsigned int line) throw (std::bad_alloc)
//    {
//      return axMallocDebug(size, file, line, 1);
//    }
//    __axmalloc_inline void* operator new[] (const size_t size,
//      const char* file, unsigned int line) throw (std::bad_alloc)
//    {
//      return axMallocDebug(size, file, line, 1);
//    }
//    __axmalloc_inline void operator delete (void* ptr) throw()
//    {
//      return axFreeDebug(ptr, ax_del_file, ax_del_line, 1);
//    }
//    __axmalloc_inline void operator delete[] (void* ptr) throw()
//    {
//     return axFreeDebug(ptr, ax_del_file, ax_del_line, 1);
//    }
//
//    #define new new(__FILE__, __LINE__)
//    #define delete if(axDebugSetDelete(__FILE__, __LINE__)) delete
//
//  #endif // AX_DEBUG_NEW
//
//#else // AX_DEBUG && AX_DEBUG_MEM
//
//  // overload operators new & delete without debug
//  #include <new>
//
//  __axmalloc_inline void* operator new (size_t size)
//    throw (std::bad_alloc)
//  {
//    return axMalloc(size);
//  }
//  __axmalloc_inline void* operator new[] (size_t size)
//    throw (std::bad_alloc)
//  {
//    return axMalloc(size);
//  }
//  __axmalloc_inline void operator delete (void* ptr) throw()
//  {
//    return axFree(ptr);
//  }
//  __axmalloc_inline void operator delete[] (void* ptr) throw()
//  {
//    return axFree(ptr);
//  }
//
//#endif // AX_DEBUG && AX_DEBUG_MEM
//
//#endif // axMalloc_included
//
