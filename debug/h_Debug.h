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
#ifndef h_Debug_included
#define h_Debug_included
//----------------------------------------------------------------------

#include "lib/h_Stdlib.h"

//----------

#ifdef H_DEBUG
  #include <iostream> // ostream
  using namespace std;
#else
  #undef H_DEBUG_LOG
  #undef H_DEBUG_CON
  #undef H_DEBUG_MEM
#endif

#ifndef H_DEBUG_MEM
  #undef H_DEBUG_NEW
#endif

//----------------------------------------------------------------------

#ifdef H_DEBUG_MEM
  #include "debug/h_MemTracer.h"
#endif

#ifdef H_DEBUG_LOG
  #include "debug/h_LogFile.h"
#endif

#ifdef H_DEBUG_CON
  #include "debug/h_Console.h"
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
#ifdef H_DEBUG
  #ifdef H_DEBUG_LOG
    #define _dbg_trace(x) \
      _trace() << x << endl; \
      m_LogFile->_trace() << x << endl;
  #else
    #define _dbg_trace(x) \
      _trace() << x << endl;
  #endif
#else
  #define _dbg_trace(x) {}
#endif
*/

//----------------------------------------

class h_Debug
{
  private:
    bool m_Initialized;

  public: //private:
    #ifdef H_DEBUG_MEM
      h_MemTracer* m_MemTracer;
    #endif
    #ifdef H_DEBUG_LOG
      h_LogFile* m_LogFile;
    #endif
    #ifdef H_DEBUG_CON
      h_Console* m_Console;
    #endif

  public:

    #ifdef H_DEBUG
      inline ostream& _trace(void) { return cout; }
    #endif

    #ifdef H_DEBUG_MEM
      //inline
      h_MemTracer* getMemTracer(void) { return m_MemTracer; }
    #endif
    #ifdef H_DEBUG_LOG
      //inline
      h_LogFile* getLogFile(void) { return m_LogFile; }
    #endif
    #ifdef H_DEBUG_CON
      //inline
      h_Console* getConsole(void) { return m_Console; }
    #endif


  public:

    h_Debug();
    ~h_Debug();
    void initialize(void);

    #ifdef H_DEBUG_MEM
      void* _malloc(size_t size);
      void* _calloc(size_t num, size_t size);
      void* _realloc(void* ptr, size_t size);
      void  _free(void* ptr);
    #endif

};

//----------------------------------------

//#undef _dbg_trace
//#undef _dbg_traceout

//----------------------------------------------------------------------

static h_Debug static_Debug;

//----------------------------------------------------------------------

#ifdef H_DEBUG_LOG
  #define TRACELOG(x) static_Debug.m_LogFile->_trace() << x << endl
#else
  #define TRACELOG(x) {}
#endif

#ifdef H_DEBUG
  #define TRACE(x) static_Debug._trace() << x << endl
#else
  #define TRACE(x) {}
#endif

//----------

#define trace(x)  TRACE(x); TRACELOG(x);
#define dtrace(x) trace(x);

//----------------------------------------------------------------------
#endif









































#if 0

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
////SUGGESTION: move to /lib
//
//#ifndef h_Debug_included
//#define h_Debug_included
///*
////----------------------------------------------------------------------
//
//
//TODO:
// - .....
// - .....
//
////----------------------------------------------------------------------
//*/
//  // pragma message
//  #define _H_DEBUG_PMSG(x)  _Pragma (#x)
//  #define H_PMSG(x)         _H_DEBUG_PMSG(message (#x))
//
//  #ifndef H_DEBUG_MEM
//    #undef H_DEBUG_NEW
//  #endif
//
//  #ifndef H_DEBUG
//    #undef H_DEBUG_CON
//    #undef H_DEBUG_LOG
//    #undef H_DEBUG_MEM
//    #undef H_DEBUG_NEW
//  #endif
//
////----------------------------------------------------------------------
//#ifdef H_DEBUG
//
//  #include "core/h_Defines.h"
//
//  #include "lib/h_BasePath.h"
//
//  #include <iostream>
//  using namespace std;
//  #include <stdlib.h>
//  #include <memory.h>
//
//  #define H_DEBUG_TEXT (char*)"_debug"
//
//  /*
//   console breaks if cout is send _after_ the console is allocated.
//   therefore for _all_ the bellow cout macros there has to be a _H_CONCHECK
//   this was a major issue in axonlib that i've tried to address it in many ways.
//   the console has to be global for this to work, but anyway it is only one
//   console per process...
//  */
//
//  #ifdef H_WIN32
//    #ifdef H_DEBUG_CON
//
//      /*
//
//        some console junk / options
//
//      */
//
//      // make console quitable (do not disable "x" btn)
//      #ifndef H_DEBUG_CON_CANQUIT
//        #define H_DEBUG_CON_CANQUIT false
//      #else
//        #undef  H_DEBUG_CON_CANQUIT
//        #define H_DEBUG_CON_CANQUIT true
//      #endif
//
//      // disable console on top
//      #ifndef H_DEBUG_CON_NOTOP
//        #define H_DEBUG_CON_NOTOP true
//      #else
//        #undef  H_DEBUG_CON_NOTOP
//        #define H_DEBUG_CON_NOTOP false
//      #endif
//
//      // disable console resize
//      #ifndef H_DEBUG_CON_NOTOP
//        #define H_DEBUG_CON_NORESIZE false
//      #else
//        #undef  H_DEBUG_CON_NORESIZE
//        #define H_DEBUG_CON_NORESIZE true
//      #endif
//
//      #include "h_DebugConsole.h"
//      #define _H_CONCHECK if (h_Con.init)
//      h_DebugConsole
//        h_Con(H_DEBUG_CON_CANQUIT, H_DEBUG_CON_NOTOP, H_DEBUG_CON_NORESIZE);
//    #else
//      #define _H_CONCHECK if (0)
//    #endif
//  #else
//    // no console for other os
//    #define _H_CONCHECK
//  #endif
//
//  #ifdef H_DEBUG_LOG
//
//    // move outside of console/log ifdef-block
//    //#include "lib/h_BasePath.h"
//
//    // hmmm, what happens if multiple instances of a plugin writes to the
//    // same fstream, at the worst imaginable moment?
//    // is cout thread-safe? probably not...
//    // so we should probably be careful with multiple debug-build instances...
//
//    /*
//      lii:
//      once a new thread starts writing to the same file stream
//      the old one stops.
//    */
//
//    #include <fstream>
//    static fstream h_LogFile;
//
//    // check for debug log append
//    #ifndef H_DEBUG_LOG_APPEND
//      #define H_DEBUG_LOG_APPEND
//    #else
//      #undef H_DEBUG_LOG_APPEND
//      #define H_DEBUG_LOG_APPEND | std::fstream::app
//    #endif
//
//    #define trace_log(x)
//      h_LogFile << "["  << h_StripPath(__FILE__)
//                << ":" << (__FUNCTION__)
//                << ":" << __LINE__ << "] "
//                << x << endl;
//
//    // "direct" output...
//    #define dtrace_log(x)
//      h_LogFile << x << endl;
//
//  #else // H_DEBUG_LOG
//
//    #define trace_log(x) {} // (void(0))
//    #define dtrace_log(x) {} // (void(0))
//
//  #endif // H_DEBUG_LOG
//
//  //--------------------------------------------------
//  // trace
//  //--------------------------------------------------
//
//  #define trace(x)
//    _H_CONCHECK
//    cout << "["  << h_StripPath(__FILE__)
//         << ":" << (__FUNCTION__)
//         << ":" << __LINE__ << "] "
//         << x << endl;
//     trace_log(x)
//
//  // "direct" output...
//  #define dtrace(x)
//    _H_CONCHECK cout << x << endl;
//    dtrace_log(x)
//
//  //--------------------------------------------------
//  // h_Debug
//  //--------------------------------------------------
//
//  //#include "lib/h_Stdlib.h"
//  //#include "lib/h_Memory.h"
//
//  #ifdef H_DEBUG_MEM
//    #include "lib/h_LeakDetector.h"
//  #else
//    #define h_Malloc  malloc
//    #define h_Calloc  calloc
//    #define h_Realloc realloc
//    #define h_Free    free
//  #endif
//
//  //------------------------------
//
//  class h_Debug
//  {
//    private:
//      #ifdef H_DEBUG_LOG
//        char h_LogFileName[H_MAX_PATH];
//      #endif
//
//    public:
//
//      h_Debug()
//        {
//          // printf("h_Debug\n");
//          #ifdef H_DEBUG_LOG
//
//            /*
//            char buffer[256];
//            h_Memset(buffer,0,sizeof(buffer));
//            char* log_basepath = h_GetBasePath(buffer);
//            h_Strcpy(h_LogFileName,log_basepath);
//            h_Strcat(h_LogFileName,H_DEBUG_LOG);
//            //trace("open_log: " << h_LogFileName);
//            */
//
//            // use "h_FromBasePath" directly
//            h_FromBasePath(h_LogFileName, H_DEBUG_LOG);
//            h_LogFile.open(h_LogFileName, std::fstream::out H_DEBUG_LOG_APPEND);
//
//          #endif
//
//          /*
//          // console has to be global to work with any macros (see above)
//            #ifdef H_DEBUG_CON
//            #endif
//            trace(""); // if logging...
//          */
//        }
//
//      //----------
//
//      ~h_Debug()
//        {
//          //printf("~h_Debug\n");
//          //trace("");
//          #ifdef H_DEBUG_MEM
//            leaks_dumpLeaks(/*true*/);
//          #endif
//          //#ifdef H_DEBUG_CON
//          //#endif
//          #ifdef H_DEBUG_LOG
//            h_LogFile.close();
//          #endif
//        }
//
//  };
//
//  /*
//
//  include h_Assert.h at this point
//  it only requires the dtrace() macro.
//
//  */
//
//  #include "core/h_Assert.h"
//
//
////----------------------------------------------------------------------
//#else // H_DEBUG
//
//  //#include <stdlib.h>
//  //#include <memory.h>
//
//  #define H_DEBUG_TEXT (char*)""
//
//  #define trace(x) {} // (void(0))
//  #define dtrace(x) {} // (void(0))
//  class h_Debug {};
//
//  #define h_Malloc  malloc
//  #define h_Calloc  calloc
//  #define h_Realloc realloc
//  #define h_Free    free
//
//#endif // H_DEBUG
////----------------------------------------------------------------------
//#endif
//
//

#endif // 0
