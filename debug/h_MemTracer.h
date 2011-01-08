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
#ifndef h_MemTracer_included
#define h_MemTracer_included
//----------------------------------------------------------------------

#include "lib/h_Memory.h"

//----------

struct h_MemNode
{
};

//----------

class h_MemTracer
{
  private:
    h_MemNode* m_Nodes;

  public:

    h_MemTracer()
      {
      }

    ~h_MemTracer()
      {
      }

    void* trace_malloc(size_t size)             { return malloc(size); }
    void* trace_calloc(size_t num, size_t size) { return calloc(num,size); }
    void* trace_realloc(void* ptr, size_t size) { return realloc(ptr,size); }
    void  trace_free(void* ptr)                 { free(ptr); }
};

//----------

#ifdef H_DEBUG_MEM
  #undef h_Malloc
  #undef h_Calloc
  #undef h_Realloc
  #undef h_Free
  #define h_Malloc  static_Debug.m_MemTracer->trace_malloc
  #define h_Calloc  static_Debug.m_MemTracer->trace_calloc
  #define h_Realloc static_Debug.m_MemTracer->trace_realloc
  #define h_Free    static_Debug.m_MemTracer->trace_free
//#else
//  h_Malloc, etc... already defined in /lib/h_Memory.h
#endif

//----------------------------------------------------------------------
#endif






















//
//
//
//
//
//
//
//
//
//
//
//
///*
//  Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al
//
//  This file is part of the Holos Library.
//  http://holos.googlecode.com
//
//  the Holos Library is free software: you can redistribute it and/or modify
//  it under the terms of the Holos Library License, either version 1.0
//  of the License, or (at your option) any later version.
//
//  the Holos Library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See LICENSE_HOLOS for more details.
//
//  You should have received a copy of the Holos Library License
//  If not, see <http://holos.googlecode.com/>.
//*/
//
//#ifndef h_LeakDetector_included
//#define h_LeakDetector_included
////----------------------------------------------------------------------
//
//// g++
//// -shared
//// -Wl,-Bsymbolic  <-- this one is necessary for the delete overloading..
//// -fPIC
//// -rdynamic
//// -DH_LADSPA
//// -DH_DEBUG
//// -I../
//// -I../../extern/ladspa_sdk_1.13 build.cpp
//// -o ../../bin/holos_debug_ladspa.so
//
//#include "core/h_Defines.h"
//#include "lib/h_BasePath.h"
//
//#define H_LEAK_MAXNAMELEN 64
//#define H_LEAK_MAXFUNCLEN 64
//#define H_LEAKS_MAXSTRING 256
//
////------------------------------
//
//struct h_LeakNode
//{
//  h_LeakNode* m_Prev;
//  h_LeakNode* m_Next;
//  void*       m_Addr;
//  int         m_Size;
//  char        m_File[H_LEAK_MAXNAMELEN];
//  char        m_Func[H_LEAK_MAXFUNCLEN];
//  int         m_Line;
//  bool        m_New;
//};
//
////----------
//
//// dangerous if multiple instances (in possibly different threads),
//// of a plugin modifies these..
//// especially the linked list..
//// todo: mutex, or something...
//
//static h_LeakNode* g_Leaks_Head         = NULL;
//static h_LeakNode* g_Leaks_Tail         = NULL;
//static int         g_Leaks_Allocated    = 0;
//static int         g_Leaks_AllocatedNum = 0;
//static int         g_Leaks_AllocatedMax = 0;
//
////----------------------------------------------------------------------
////
////----------------------------------------------------------------------
//
//// append to tail...
//void leaks_appendNode(h_LeakNode* a_Node)
//  {
//    if (g_Leaks_Tail)
//    {
//      // xx XX --
//      a_Node->m_Prev = g_Leaks_Tail;
//      a_Node->m_Next = NULL;
//      g_Leaks_Tail->m_Next = a_Node;
//      g_Leaks_Tail = a_Node;
//    }
//    else
//    {
//      // -- XX --
//      g_Leaks_Head = a_Node;
//      a_Node->m_Prev = NULL;
//      a_Node->m_Next = NULL;
//      g_Leaks_Tail = a_Node;
//    }
//    g_Leaks_Allocated += a_Node->m_Size;
//    g_Leaks_AllocatedNum++;
//    if (g_Leaks_Allocated > g_Leaks_AllocatedMax) g_Leaks_AllocatedMax = g_Leaks_Allocated;
//  }
//
////----------
//
//// remove node
//void leaks_removeNode(h_LeakNode* a_Node)
//  {
//    h_LeakNode* next = a_Node->m_Next;
//    h_LeakNode* prev = a_Node->m_Prev;
//    if (prev)
//    {
//      if (next)
//      {
//        //  xx XX xx
//        prev->m_Next = next;
//        next->m_Prev = prev;
//      }
//      else
//      {
//        // xx XX -- (tail)
//        prev->m_Next = NULL;
//        g_Leaks_Tail = prev;
//      }
//    }
//    else // !head
//    {
//      if (next)
//      {
//        // -- XX xx (head)
//        g_Leaks_Head = next;
//        next->m_Prev = NULL;
//      }
//      else
//      {
//        // -- XX -- (single)
//        g_Leaks_Head = NULL;
//        g_Leaks_Tail = NULL;
//      }
//    } // !next
//    g_Leaks_Allocated -= a_Node->m_Size;
//    g_Leaks_AllocatedNum--;
//    //m_FreedNum++;
//  }
//
////----------------------------------------------------------------------
////
////----------------------------------------------------------------------
//
//#include "h_Stdlib.h"
//
//void leaks_appendLeak(void* a_Addr, unsigned int a_Size, const char* a_File, const char* a_Func, unsigned int a_Line, bool a_New)
//  {
//    const char* file = h_StripPath(a_File);
//    h_LeakNode* node = (h_LeakNode*)malloc(sizeof(h_LeakNode)); // !!!
//    node->m_Addr = a_Addr;
//    node->m_Size = a_Size;
//    h_Strncpy(node->m_File, file ,H_LEAKS_MAXSTRING-1);
//    h_Strncpy(node->m_Func, a_Func ,H_LEAKS_MAXSTRING-1);
//    node->m_Line = a_Line;
//    node->m_New = a_New;
//    leaks_appendNode(node);
//    #ifdef H_DEBUG_MEM_PRINT
//      dtrace( "+ ["  << file << ":"  << a_Func << ":"  << a_Line << "] "  <<
//            (a_New?"new":"h_Malloc") << ", " << a_Addr << ", " << a_Size);
//    #endif
//  }
//
////----------
//
//void leaks_removeLeak(void* a_Addr, __unused const char* a_File, __unused const char* a_Func, __unused unsigned int a_Line, __unused bool a_New)
//  {
//    h_LeakNode* node = g_Leaks_Head;
//    while (node)
//    {
//      if (node->m_Addr==a_Addr) break;
//      node = (h_LeakNode*)node->m_Next;
//    }
//    if (node)
//    {
//      leaks_removeNode(node);
//      #ifdef H_DEBUG_MEM_PRINT
//        //const char* file = node->m_File;//h_StripPath(node->m_File);
//        dtrace( "- ["  << node->m_File << ":"  << node->m_Func << ":"  << node->m_Line << "] "  <<
//              (node->m_New?"delete":"h_Free") << ", " << node->m_Addr << ", " << node->m_Size);
//      #endif
//      free(node); // !!!
//    }
//  }
//
////----------
//
//void leaks_dumpLeaks(void/*bool a_Delete*/)
//  {
//    dtrace("--------------------------------------------------");
//    dtrace("* num leaks:     " << g_Leaks_AllocatedNum);
//    dtrace("* max allocated: " << g_Leaks_AllocatedMax);
//    dtrace("* leaked memory: " << g_Leaks_Allocated);
//    if (g_Leaks_Allocated > 0)
//    {
//      dtrace("--------------------------------------------------");
//      h_LeakNode* node = (h_LeakNode*)g_Leaks_Head;
//      while (node)
//      {
//        dtrace( "* [" << node->m_File <<
//                ":"   << node->m_Func <<
//                ":"   << node->m_Line <<
//                "]"   <<
//                " "   << (node->m_New?"new":"h_Malloc") <<
//                ", "  << node->m_Addr <<
//                ", "  << node->m_Size);
//        h_LeakNode* next = (h_LeakNode*)node->m_Next;
//        //if (a_Delete) { free(node); }
//        node = next;
//      }
//    }
//    dtrace("--------------------------------------------------");
//  }
//
////----------------------------------------------------------------------
////
////----------------------------------------------------------------------
//
//void* leaks_malloc(const size_t size, const char* file, const char* func, unsigned int line)
//  {
//    //trace("_malloc");
//    void* ptr = malloc(size);
//    leaks_appendLeak(ptr,size,file,func,line,false);
//    return ptr;
//  }
//
////----------
//
//void* leaks_calloc(const int num, const size_t size, const char* file, const char* func, unsigned int line)
//  {
//
//    //trace("_calloc");
//    void* ptr = calloc(num,size);
//    leaks_appendLeak(ptr,num*size,file,func,line,false);
//    return ptr;
//  }
//
////----------
//
////TODO: resize
//void* leaks_realloc(void* ptr, const size_t size, const char* file, const char* func, unsigned int line)
//  {
//    //trace("_realloc");
//    void* p2 = realloc(ptr,size);
//    leaks_removeLeak(ptr,file,func,line,false);
//    leaks_appendLeak(p2,size,file,func,line,false);
//    return p2;
//  }
//
////----------
//
//void leaks_free(void* ptr, const char* file, const char* func, unsigned int line)
//  {
//    //trace("_free");
//    leaks_removeLeak(ptr,/*size,*/file,func,line,false);
//    free(ptr);
//  }
//
////----------------------------------------------------------------------
//
//// ??
///*
//#define h_Malloc(size)      leaks_malloc(size,     __FILE__,__FUNCTION__,__LINE__)
//#define h_Calloc(num,size)  leaks_calloc(num,size, __FILE__,__FUNCTION__,__LINE__)
//#define h_Realloc(ptr,size) leaks_realloc(ptr,size,__FILE__,__FUNCTION__,__LINE__)
//#define h_Free(ptr)         leaks_free(ptr,        __FILE__,__FUNCTION__,__LINE__)
//*/
//
////----------------------------------------------------------------------
////
////----------------------------------------------------------------------
//
//// http://www.sannabremo.se/nyquist/industrial/IndustrialStrength.d.html
//
//// The scope of the operator new used by the new -expression is searched for a
//// matching operator delete . A declaration of an operator delete matches the
//// declaration of a operator new when it has the same number of parameters and
//// all parameter types except the first are identical. The run-time system will
//// then call the matching operator de lete to deallocate a partially
//// constructed object.
//
//#ifdef H_DEBUG_NEW
//
//  // these should be defined as __thread
//  static __thread char*         leaks_del_file = (char*)"none";
//  static __thread char*         leaks_del_func = (char*)"none";
//  static __thread unsigned int  leaks_del_line = 666;
//
//  //----------
//
//  // this is being called, even if the wrong 'delete' operator is used
//
//  bool leaks_del_prepare(const char* file, const char* func, const unsigned int line)
//    {
//      //dtrace("leaks_del_prepare:" /*<< h_StripPath(file) << "," << func << "," << line*/);
//      leaks_del_file = (char*)file;
//      leaks_del_func = (char*)func;
//      leaks_del_line = line;
//      return true;
//    }
//
//  void* operator new (const size_t size, const char* file, const char* func, unsigned int line) throw (std::bad_alloc)
//    {
//      //dtrace("op new");
//      void* ptr = malloc(size);
//      leaks_appendLeak(ptr,size,file,func,line,true);
//      return ptr;
//    }
//
//  void* operator new[] (const size_t size, const char* file, const char* func, unsigned int line) throw (std::bad_alloc)
//    {
//      //dtrace("op new[]");
//      void* ptr = malloc(size);
//      leaks_appendLeak(ptr,size,file,func,line,true);
//      return ptr;
//    }
//
//  void operator delete (void* ptr) throw()
//    {
//      //dtrace("op delete");
//      free(ptr);
//      leaks_removeLeak(ptr,leaks_del_file,leaks_del_func,leaks_del_line,true);
//    }
//
//  void operator delete[] (void* ptr) throw()
//    {
//      //dtrace("op delete[]");
//      free(ptr);
//      leaks_removeLeak(ptr,leaks_del_file,leaks_del_func,leaks_del_line,true);
//    }
//
//
//  /*
//    void* operator new (const size_t size, const std::nothrow_t& nothrow_constant, const char* file, const char* func, unsigned int line) throw()
//    {
//      dtrace("op new nothrow");
//      void* ptr = malloc(size);
//      leaks_appendLeak(ptr,size,file,func,line,true);
//      return ptr;
//    }
//
//    void* operator new[] (const size_t size, const std::nothrow_t& nothrow_constant, const char* file, const char* func, unsigned int line) throw()
//    {
//      dtrace("op new[] nothrow");
//      void* ptr = malloc(size);
//      leaks_appendLeak(ptr,size,file,func,line,true);
//      return ptr;
//    }
//
//    void operator delete (void* ptr, const std::nothrow_t& nothrow_constant) throw()
//    {
//      dtrace("op delete nothrow");
//      free(ptr);
//      leaks_removeLeak(ptr,leaks_del_file,leaks_del_func,leaks_del_line,true);
//    }
//
//  void operator delete[] (void* ptr, const std::nothrow_t& nothrow_constant) throw()
//    {
//      dtrace("op delete[] nothrow");
//      free(ptr);
//      leaks_removeLeak(ptr,leaks_del_file,leaks_del_func,leaks_del_line,true);
//    }
//
//  */
//
//// catch malloc
//#define malloc(size)      leaks_malloc(size,     __FILE__,__FUNCTION__,__LINE__)
//#define calloc(num,size)  leaks_calloc(num,size, __FILE__,__FUNCTION__,__LINE__)
//#define realloc(ptr,size) leaks_realloc(ptr,size,__FILE__,__FUNCTION__,__LINE__)
//#define free(ptr)         leaks_free(ptr,        __FILE__,__FUNCTION__,__LINE__)
//
//// ????
//#define h_Malloc            malloc
//#define h_Calloc            calloc
//#define h_Realloc           realloc
//#define h_Free              free
//
//// catch op. new / delete
//#define new new(__FILE__, __FUNCTION__, __LINE__)
//#define delete if (leaks_del_prepare(__FILE__, __FUNCTION__, __LINE__)) delete
//
//  /*
//  ^
//  -----------------
//  lii:
//    (edit after a bit of testing)
//    the leak detector will not work well with the "nothrow" versions.
//    this is due to the macro expansion:
//      int* p = new(std::notrow) int;
//    will expand to something like:
//      int* p = new(__FILE__, __FUNCTION__, __LINE__)(std::notrow) int;
//
//  here is one possible solution - needs thread safety:
//  http://groups.google.com/group/comp.lang.c++.moderated/browse_thread/thread/7089382e3bc1c489/85f9107a1dc79ee9?pli=1
//  (and can't get it to work)
//
//  so i suggest we leave the 'nothrow' versions out and document that such will
//  not work with the debugger due to compile errors.
//  -----------------
//  */
//
//  /*
//
//  aargh!!
//  doesn't seem the overloaded delete is called in a dll!
//  it works nicely in an exe.. 'op delete' is printed (see trace above))
//  but in a dll (vst), 'leaks_del_delete' is printed out, but not the 'op delete'..
//  (as if a different 'delete' is being called... hmmm)
//
//  leaks_del_prepare is printed, so our 'delete' macro is being used,
//  but not the correct delete operator..
//
//  could we call leaks_removeLeak directly, in the macro instead?
//  or, could that cause problems because of the various different
//  delete operators, like throw/nothrow, [], etc
//
//  (or perhaps in the leaks_del_prepare function?)
//
//  -----------------
//  lii:
//    leaks_removeLeak should be called inside the overloaded op function,
//    since only this way the correect pointer 'ptr' can be passed to it ?
//    leaks_del_prepare() is used so that the
//    (__FILE__,__FUNCTION__,__LINE__) info is passed correctly avoiding the
//    syntax issues of _both_ delete ops.
//    and leaks_del_prepare is called before the delete op inside the
//    if () branch.
//
//  -----------------
//
//  if the wrong 'delete' is called, the memory isn't released correctly?
//  allocated with malloc, free'd with delete, no-good...
//  (but then, this is only for debugging)
//
//  */
//
//
//
//  //#define delete  dtrace("!!! delete !!!");
//  //#define delete  debug_ext::debug_mem_allocation(__FILE__,__LINE__, __FUNCTION__) +
//
//#endif // H_DEBUG_NEW
////----------------------------------------------------------------------
//#endif
