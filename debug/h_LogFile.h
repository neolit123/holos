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
#ifndef h_LogFile_included
#define h_LogFile_included
//----------------------------------------------------------------------

#include <iostream>
using namespace std;
#include <fstream>

#include "lib/h_BasePath.h"

//----------------------------------------------------------------------

#ifndef H_DEBUG_LOG_APPEND
  #define H_DEBUG_LOG_APPEND
#else
  #undef H_DEBUG_LOG_APPEND
  #define H_DEBUG_LOG_APPEND | std::fstream::app
#endif

//----------------------------------------------------------------------

class h_LogFile
{
  private:
    char    m_LogFileName[H_MAX_PATHSIZE];
    fstream m_LogFile; // static?

  public:

    inline fstream& _trace(void) { return m_LogFile; }

  public:

    h_LogFile(const char* a_FileName)
      {
        m_LogFileName[0] = '\0';
/* */   h_GetBasePath(m_LogFileName);
        h_Strcat(m_LogFileName,a_FileName/*H_DEBUG_LOG*/);
        m_LogFile.open(m_LogFileName, std::fstream::out H_DEBUG_LOG_APPEND);
      }

    ~h_LogFile()
      {
        m_LogFile.close();
      }

    //void print(const char* x)
    //  {
    //    m_LogFile << x << endl;
    //  }

};

//----------------------------------------------------------------------
#endif
