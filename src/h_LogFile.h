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
#include <fstream>
using namespace std;

#include "src/h_BasePath.h"

#ifdef H_DEBUG_LOG_UNIQUE
  #include "src/h_Timestamp.h"
#endif

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
    char          m_LogFileName[H_MAX_PATHSIZE];
    fstream       m_LogFile; // static?
    
    #ifdef H_DEBUG_LOG_UNIQUE
      h_Timestamp m_TS;
    #endif

  public:

    inline fstream& _trace(void) { return m_LogFile; }

  public:

    h_LogFile(const char* a_FileName)
      {
        #ifndef H_DEBUG_LOG_HOME
          // try writing to base path
          
          m_LogFileName[0] = '\0';
          h_GetBasePath(m_LogFileName);
          #ifdef H_DEBUG_LOG_UNIQUE
            h_Strcat(m_LogFileName, m_TS.str());
            h_Strcat(m_LogFileName, (char*)"_");
          #endif
          
          h_Strcat(m_LogFileName, a_FileName);
          m_LogFile.open(m_LogFileName, std::fstream::out H_DEBUG_LOG_APPEND);
          
          if (m_LogFile.fail())
          {
            printf( "ERROR: cannot write log to base path: %s\n",
                  m_LogFileName);
        #endif
            // write to home path instead
            
            m_LogFileName[0] = '\0';
            h_GetHomePath(m_LogFileName);
            #ifdef H_DEBUG_LOG_UNIQUE
              h_Strcat(m_LogFileName, m_TS.str());
              h_Strcat(m_LogFileName, (char*)"_");
            #endif
            
            h_Strcat(m_LogFileName, a_FileName);
            m_LogFile.open(m_LogFileName, std::fstream::out H_DEBUG_LOG_APPEND);
  
            if (m_LogFile.fail())
            {
              printf( "ERROR: cannot write log to home path: %s\n",
                    m_LogFileName);
              m_LogFile.clear();
            }
        #ifndef H_DEBUG_LOG_HOME
          }
        #endif
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
