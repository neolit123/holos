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

// g++
// -shared
// -Wl,-Bsymbolic  <-- this one is necessary for the delete overloading..
// -fPIC
// -rdynamic
// -DH_LADSPA
// -DH_DEBUG
// -I../
// -I../../extern/ladspa_sdk_1.13 build.cpp
// -o ../../bin/holos_debug_ladspa.so

//----------
//
// some interesting things:
// http://eli.thegreenplace.net/2011/02/17/the-many-faces-of-operator-new-in-c/
//

//----------------------------------------------------------------------

#include "src/h_Memory.h"
//#include "src/h_Stdlib.h"

/*
  in case we want to change them...
  by not using h_Malloc, etc, we avoid keeping track of the small allocs for
  the mem nodes, and also for the proper/real allocations
*/

#define leak_malloc   malloc
#define leak_calloc   calloc
#define leak_realloc  realloc
#define leak_free     free

struct h_MemNode
{
  h_MemNode* m_Prev;
  h_MemNode*  m_Next;
  void*       m_Addr;
  int         m_Size;
  char        m_File[H_MAX_STRINGSIZE+1];
  char        m_Func[H_MAX_STRINGSIZE+1];
  int         m_Line;
  bool        m_New;
};

//----------------------------------------------------------------------

class h_MemTracer
{
  private:
    bool        m_Started;
    h_MemNode*  m_Head;
    h_MemNode*  m_Tail;
    int         m_Allocated;
    int         m_AllocatedNum;
    int         m_AllocatedMax;
    int         m_NumAlloc;
    int         m_NumFree;

  public:

    h_MemTracer()
      {
        m_Started       = false;
        m_Head          = H_NULL;
        m_Tail          = H_NULL;
        m_Allocated     = 0;
        m_AllocatedNum  = 0;
        m_AllocatedMax  = 0;
        m_NumAlloc      = 0;
        m_NumFree       = 0;
      }

    ~h_MemTracer()
      {
        dump();
      }

    void start(void)
      {
        m_Started = true;
      }

    void stop(void)
      {
        m_Started = false;
      }

  private:

    // [internal]
    // append to tail...
    void _appendNode(h_MemNode* a_Node)
      {
        if (m_Tail)
        {
          // xx XX --
          a_Node->m_Prev = m_Tail;
          a_Node->m_Next = H_NULL;
          m_Tail->m_Next = a_Node;
          m_Tail = a_Node;
        }
        else
        {
          // -- XX --
          m_Head = a_Node;
          a_Node->m_Prev = H_NULL;
          a_Node->m_Next = H_NULL;
          m_Tail = a_Node;
        }
        m_Allocated += a_Node->m_Size;
        m_AllocatedNum++;
        if (m_Allocated > m_AllocatedMax) m_AllocatedMax = m_Allocated;
      }

    //[internal]
    // remove node
    void _removeNode(h_MemNode* a_Node)
      {
        h_MemNode* next = a_Node->m_Next;
        h_MemNode* prev = a_Node->m_Prev;
        if (prev)
        {
          if (next)
          {
            //  xx XX xx
            prev->m_Next = next;
            next->m_Prev = prev;
          }
          else
          {
            // xx XX -- (tail)
            prev->m_Next = NULL;
            m_Tail = prev;
          }
        }
        else // !head
        {
          if (next)
          {
            // -- XX xx (head)
            m_Head = next;
            next->m_Prev = NULL;
          }
          else
          {
            // -- XX -- (single)
            m_Head = NULL;
            m_Tail = NULL;
          }
        } // !next
        m_Allocated -= a_Node->m_Size;
        m_AllocatedNum--;
        //m_FreedNum++;
      }

  public:

    void append(void* a_Addr, unsigned int a_Size, const char* a_File, const char* a_Func, unsigned int a_Line, bool a_New)
      {
        if (m_Started)
        {
          const char* file = h_StripPath(a_File);
          h_MemNode* node = (h_MemNode*)leak_malloc(sizeof(h_MemNode)); // !!!
          m_NumAlloc++;
          node->m_Addr = a_Addr;
          node->m_Size = a_Size;
          h_Strncpy(node->m_File, file ,H_MAX_STRINGSIZE);
          h_Strncpy(node->m_Func, a_Func ,H_MAX_STRINGSIZE);
          node->m_Line = a_Line;
          node->m_New = a_New;
          _appendNode(node);
          #ifdef H_DEBUG_MEM_PRINT
            dtrace( "+ ["  << file << ":" << a_Line << ":" << a_Func << "] " <<
                  (a_New?"new":"h_Malloc") << ", " << a_Addr << ", " << a_Size );
          #endif
        }
      }

    void remove(void* a_Addr, __unused const char* a_File, __unused const char* a_Func, __unused unsigned int a_Line, __unused bool a_New)
      {
        if (m_Started)
        {
          const char* file = h_StripPath(a_File);
          h_MemNode* node = m_Head;
          while (node)
          {
            if (node->m_Addr==a_Addr) break;
            node = (h_MemNode*)node->m_Next;
          }
          if (node)
          {
            _removeNode(node);
            #ifdef H_DEBUG_MEM_PRINT
              dtrace("- ["  << file << ":" << a_Line << ":" << a_Func << "] "
                     << (a_New?"delete, ":"h_Free, ")
                     << node->m_Addr << ", " << node->m_Size);
              if (node->m_New != a_New)
              {
                dtrace("! mismatch!!! ["
                       << node->m_File << ":" << node->m_Line << ":" << node->m_Func << "] "
                       << (node->m_New?"new":"h_Malloc") );
              }
            #endif
            leak_free(node); // !!!
            m_NumFree++;
          }
          //else
          //{
          //  dtrace("! node not found !");
          //  // and it will probably crash/segfault...
          //}
        }
      }

    void dump(void)
      {
        dtrace("--------------------------------------------------");
        dtrace("* num alloc:     " << m_NumAlloc);
        dtrace("* num free       " << m_NumFree);
        dtrace("* num leaks:     " << m_AllocatedNum);
        dtrace("* max allocated: " << m_AllocatedMax);
        dtrace("* leaked memory: " << m_Allocated);
        if (m_Allocated > 0)
        {
          dtrace("--------------------------------------------------");
          h_MemNode* node = (h_MemNode*)m_Head;
          while (node)
          {
            dtrace( "* [" << node->m_File <<
                    ":"   << node->m_Func <<
                    ":"   << node->m_Line <<
                    "]"   <<
                    " "   << (node->m_New?"new":"h_Malloc") <<
                    ", "  << node->m_Addr <<
                    ", "  << node->m_Size);
            h_MemNode* next = (h_MemNode*)node->m_Next;
            //if (a_Delete) { free(node); }
            node = next;
          }
        }
        dtrace("--------------------------------------------------");
      }

    void* trace_malloc(const size_t size, const char* file, const char* func, unsigned int line)
      {
        void* ptr = leak_malloc(size);
        append(ptr,size,file,func,line,false);
        return ptr;
      }

    void* trace_calloc(const int num, const size_t size, const char* file, const char* func, unsigned int line)
      {
        void* ptr = leak_calloc(num,size);
        append(ptr,num*size,file,func,line,false);
        return ptr;
      }

    void* trace_realloc(void* ptr, const size_t size, const char* file, const char* func, unsigned int line)
      {
        void* p2 = leak_realloc(ptr,size);
        remove(ptr,file,func,line,false);
        append(p2,size,file,func,line,false);
        return p2;
      }

    void trace_free(void* ptr, const char* file, const char* func, unsigned int line)
      {
        remove(ptr,file,func,line,false);
        leak_free(ptr);
      }

};

//----------------------------------------------------------------------

// being static means that multiple instances uses the same
// static_MemTracer object.. it's shared for all instances/threads..
//
// needs to be static, so our 'overloaded' new/delete macros can use it
// would be better if it were part of the h_Debug class, but i haven't found
// a nice/clean way to replace the new/delete with macros that call a
// non-static class method. if this were part of a class, this class must be
// available/accessible during our new (pun..) new/delete macros. everywhere.

// __thread ??

// multiple instances can be in a single thread, or in multiple threads.
// our problem above is not related to threads, but that many plugin instances
// have access to the same static/global class-instance.

static h_MemTracer static_MemTracer;

//----------------------------------------------------------------------

#ifdef H_DEBUG_MEM

  #ifdef H_DEBUG_NEW

    // unreliable (and dangerous?) if multiple plugin instances uses
    // mem-tracing at the same time..

    // these should be defined as __thread
    static __thread char*         mem_del_file;
    static __thread char*         mem_del_func;
    static __thread unsigned int  mem_del_line;

    bool mem_del_prepare(const char* file, const char* func, const unsigned int line)
      {
        //dtrace("leaks_del_prepare:" /*<< h_StripPath(file) << "," << func << "," << line*/);
        mem_del_file = (char*)file;
        mem_del_func = (char*)func;
        mem_del_line = line;
        return true;
      }

    void* operator new (const size_t size, const char* file, const char* func, unsigned int line) throw (std::bad_alloc)
      {
        //dtrace("op new");
        void* ptr = leak_malloc(size);
        static_MemTracer.append(ptr,size,file,func,line,true);
        return ptr;
      }

    void* operator new[] (const size_t size, const char* file, const char* func, unsigned int line) throw (std::bad_alloc)
      {
        //dtrace("op new[]");
        void* ptr = leak_malloc(size);
        static_MemTracer.append(ptr,size,file,func,line,true);
        return ptr;
      }

    void operator delete (void* ptr) throw()
      {
        //dtrace("op delete");
        leak_free(ptr);
        static_MemTracer.remove(ptr,mem_del_file,mem_del_func,mem_del_line,true);
      }

    void operator delete[] (void* ptr) throw()
      {
        //dtrace("op delete[]");
        leak_free(ptr);
        static_MemTracer.remove(ptr,mem_del_file,mem_del_func,mem_del_line,true);
      }

  #endif // H_DEBUG_NEW

  // needed ??
  #undef h_Malloc
  #undef h_Calloc
  #undef h_Realloc
  #undef h_Free

  #ifdef H_DEBUG_MEM_NOREDEFINE
    #define h_Malloc(size)      static_MemTracer.trace_malloc(size,     __FILE__,__FUNCTION__,__LINE__)
    #define h_Calloc(num,size)  static_MemTracer.trace_calloc(num,size, __FILE__,__FUNCTION__,__LINE__)
    #define h_Realloc(ptr,size) static_MemTracer.trace_realloc(ptr,size,__FILE__,__FUNCTION__,__LINE__)
    #define h_Free(ptr)         static_MemTracer.trace_free(ptr,        __FILE__,__FUNCTION__,__LINE__)
  #else
    // catch malloc
    #define malloc(size)      static_MemTracer.trace_malloc(size,     __FILE__,__FUNCTION__,__LINE__)
    #define calloc(num,size)  static_MemTracer.trace_calloc(num,size, __FILE__,__FUNCTION__,__LINE__)
    #define realloc(ptr,size) static_MemTracer.trace_realloc(ptr,size,__FILE__,__FUNCTION__,__LINE__)
    #define free(ptr)         static_MemTracer.trace_free(ptr,        __FILE__,__FUNCTION__,__LINE__)
    // ????
    #define h_Malloc    malloc
    #define h_Calloc    calloc
    #define h_Realloc   realloc
    #define h_Free      free
  #endif

  #ifdef H_DEBUG_NEW

    // catch op. new / delete
    #define new new(__FILE__, __FUNCTION__, __LINE__)
    #define delete if (mem_del_prepare(__FILE__, __FUNCTION__, __LINE__)) delete

  #endif // H_DEBUG_NEW

//#else
//  h_Malloc, etc... already defined in /lib/h_Memory.h
#endif

//----------

#undef leak_malloc
#undef leak_calloc
#undef leak_realloc
#undef leak_free

//----------------------------------------------------------------------
#endif

