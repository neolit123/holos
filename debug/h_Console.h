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

class h_Console
{
  private:

  public:

    h_Console()
      {
      }

    ~h_Console()
      {
      }

};

//----------------------------------------------------------------------
#endif































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
//#ifndef h_DebugConsole_included
//#define h_DebugConsole_included
//
//#include "core/h_Types.h"
//
//#if defined H_WIN32 && defined H_DEBUG && defined H_DEBUG_CON
//
//#include <windows.h>
//#include <io.h>
//#include <stdio.h>
//
//#include "lib/h_DetectWine.h"
//
//class h_DebugConsole
//{
//  private:
//    FILE* h_Sfile;  // file stream
//
//  public:
//    bool init;
//
//    h_DebugConsole(const bool xbtn = false, const bool ontop = true,
//      const bool resize = true)
//    {
//      AllocConsole();
//      SetConsoleTitle("h_Debug");
//      HWND hCw = GetConsoleWindow();
//      if (hCw)
//      {
//        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
//        // ENABLE_EXTENDED_FLAGS = 0x0080, ENABLE_QUICK_EDIT_MODE = 0x0040
//        SetConsoleMode(hIn,0x0080|0x0040);
//        SetConsoleCtrlHandler(NULL,true);
//        // resize
//        if (resize)
//        {
//          HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//          SMALL_RECT cDim = {0,0,80,20};
//          SetConsoleWindowInfo(hOut,true,&cDim);
//         }
//        // ------------
//        // detect wine (SetWindowPos WINE bug here !!)
//        if (!h_DetectWine())
//          if (ontop)
//            SetWindowPos(hCw,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
//        // ------------
//	      HMENU hMenu = GetSystemMenu(hCw,0);
//	      if (hMenu)
//	      {
//          if (!xbtn)
//          {
//            DeleteMenu(hMenu,SC_CLOSE,MF_BYCOMMAND);
//            DrawMenuBar(hCw);
//          }
//	      }
//        else
//          printf("[h_DebugConsole.h]: winapi console: # cannot get menu\n");
//        // _O_TEXT = 0x4000
//        h_intptr h_Hcrt =
//          _open_osfhandle((h_intptr)GetStdHandle(STD_OUTPUT_HANDLE), 0x4000);
//        if (h_Hcrt)
//        {
//          h_Sfile = _fdopen(h_Hcrt,"w");
//          if (h_Sfile)
//          {
//            *stdout = *h_Sfile;
//            setvbuf(stdout,NULL,_IONBF,0);
//            init = 1;
//
//            printf("[h_DebugConsole.h] winapi console: ok\n");
//          }
//        }
//        else
//          printf("[h_DebugConsole.h] winapi console: # cannot route stdout\n");
//      }
//      else
//        printf("[h_DebugConsole.h] winapi console: # cannot allocate\n");
//    }
//
//    ~h_DebugConsole()
//    {
//      FreeConsole();
//      if (h_Sfile)
//        close((h_intptr)h_Sfile);
//    }
//
//};
//
//#endif // defined H_WIN32 && defined H_DEBUG && defined H_DEBUG_AUTOSTD
//
//#endif // h_DebugConsole_included
//
//
//
