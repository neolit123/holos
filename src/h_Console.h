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
#ifndef h_Console_included
#define h_Console_included
//----------------------------------------------------------------------

//#if defined H_WIN32 && defined H_DEBUG && defined H_DEBUG_CON

#include <windows.h>
#include <io.h>
#include <stdio.h>

#include "src/h_Types.h"
#include "src/h_DetectWine.h"

//----------

// make console quitable (do not disable "x" btn)
#ifdef H_DEBUG_CON_CANQUIT
  #undef  H_DEBUG_CON_CANQUIT
  #define H_DEBUG_CON_CANQUIT true
#else
  #define H_DEBUG_CON_CANQUIT false
#endif

// disable console on top
#ifdef H_DEBUG_CON_NOTOP
  #undef  H_DEBUG_CON_NOTOP
  #define H_DEBUG_CON_NOTOP true
#else
  #define H_DEBUG_CON_NOTOP false
#endif

// disable console resize
#ifdef H_DEBUG_CON_NORESIZE
  #undef  H_DEBUG_CON_NORESIZE
  #define H_DEBUG_CON_NORESIZE true
#else
  #define H_DEBUG_CON_NORESIZE false
#endif

//----------

class h_Console
{
  private:
    bool    m_Initialized;
    FILE*   m_File;
  public:
    //bool    init;

  public:

    h_Console()
//              bool xbtn=false,
//              bool ontop=true,
//              bool resize=true)
      {

        bool xbtn   = H_DEBUG_CON_CANQUIT;
        bool ontop  = !H_DEBUG_CON_NOTOP;
        bool resize = !H_DEBUG_CON_NORESIZE;

        m_Initialized = false;
        m_File = H_NULL;
        AllocConsole();
        SetConsoleTitle("h_Debug");
        HWND hCw = GetConsoleWindow();
        if (hCw)
        {
          HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
          // ENABLE_EXTENDED_FLAGS = 0x0080, ENABLE_QUICK_EDIT_MODE = 0x0040
          SetConsoleMode(hIn,0x0080|0x0040);
          SetConsoleCtrlHandler(NULL,true);
          // resize
          if (resize)
          {
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            SMALL_RECT cDim = {0,0,80,20};
            SetConsoleWindowInfo(hOut,true,&cDim);
          }
          // ------------
          // detect wine (SetWindowPos WINE bug here !!)
          if (!h_DetectWine())
            if (ontop)
              SetWindowPos(hCw,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
          // ------------
          HMENU hMenu = GetSystemMenu(hCw,0);
          if (hMenu)
          {
            if (!xbtn)
            {
              DeleteMenu(hMenu,SC_CLOSE,MF_BYCOMMAND);
              DrawMenuBar(hCw);
            }
          }
          //else printf("[h_DebugConsole.h]: winapi console: # cannot get menu\n");
          // _O_TEXT = 0x4000
          h_uintptr_t h_Hcrt = _open_osfhandle((h_uintptr_t)GetStdHandle(STD_OUTPUT_HANDLE), 0x4000);
          if (h_Hcrt)
          {
            m_File = _fdopen(h_Hcrt,"w");
            if (m_File)
            {
              *stdout = *m_File;
              setvbuf(stdout,NULL,_IONBF,0);
              m_Initialized = 1;
              //printf("[h_DebugConsole.h] winapi console: ok\n");
            }
          }
          //else printf("[h_DebugConsole.h] winapi console: # cannot route stdout\n");
        }
        //else printf("[h_DebugConsole.h] winapi console: # cannot allocate\n");
      }

    //----------

    ~h_Console()
      {
        FreeConsole();
        if (m_File) close((h_uintptr_t)m_File);
      }

    //----------

};

//#endif // defined H_WIN32 && defined H_DEBUG && defined H_DEBUG_AUTOSTD

//----------------------------------------------------------------------
#endif
