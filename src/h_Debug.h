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

#include "src/h_Stdlib.h"
#include "src/h_Utils.h"  // h_StripPath

//----------------------------------------------------------------------

// pragma message
#define _h_pmsg(x)  _Pragma (#x)
#define h_Pmsg(x)   _h_pmsg(message (#x))

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

#ifdef H_DEBUG_LOG
  #include "src/h_LogFile.h"
#endif

#ifdef H_DEBUG_CON
  #include "src/h_Console.h"
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

class h_Debug
{
  private:
    bool m_Initialized;

  public: //private:
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

};

//#undef _dbg_trace
//#undef _dbg_traceout

//----------------------------------------------------------------------

static h_Debug static_Debug;

//----------------------------------------------------------------------

#ifdef H_DEBUG_LOG
  #define TRACELOG(x) \
    static_Debug.m_LogFile->_trace() << "[" \
                          << h_StripPath(__FILE__) << ":" \
                          << __FUNCTION__ << ": " \
                          << __LINE__ << "] " \
                          << x << endl
  #define DTRACELOG(x) static_Debug.m_LogFile->_trace() << x << endl
#else
  #define TRACELOG(x) {}
  #define DTRACELOG(x) {}
#endif

//----------

#ifdef H_DEBUG
  #define TRACE(x) \
    static_Debug._trace() << "[" \
                          << h_StripPath(__FILE__) << ":" \
                          << __FUNCTION__ << ":" \
                          << __LINE__ << "] " \
                          << x << endl
  #define DTRACE(x) static_Debug._trace() << x << endl
#else
  #define TRACE(x)  {}
  #define DTRACE(x) {}
#endif

//----------

#define trace(x)  TRACE(x);  TRACELOG(x);
#define dtrace(x) DTRACE(x); DTRACELOG(x);

//----------

#include "src/h_Assert.h"

#ifdef H_DEBUG_MEM
  #include "src/h_MemTracer.h"
#endif

//----------------------------------------------------------------------
#endif
























