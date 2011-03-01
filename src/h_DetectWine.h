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

#ifndef h_DetectWine_included
#define h_DetectWine_included
//----------------------------------------------------------------------

#ifdef H_WIN32

#include "src/h_Defines.h" // sets winver etc..
#include <windows.h>

// detect wine
BOOL h_DetectWine(void)
  {
    BOOL ret = 0;
    HINSTANCE hLib = LoadLibrary(TEXT("ntdll.dll"));
    if (hLib)
    {
      ret = GetProcAddress(hLib, "wine_get_version") != NULL ||
            GetProcAddress(hLib, "wine_nt_to_unix_file_name") != NULL;
      /*BOOL unload =*/ FreeLibrary(hLib);
      //if (!unload)
      //  printf("[h_DetectWine.h] wine detect: # cannot release ntdll.dll\n");
    }
    //else
    //  printf("[h_DetectWine.h] wine detect: # cannot load ntdll.dll\n");
    //printf("[h_DetectWine.h] wine detect: %d\n", ret);
    return ret;
  }

#endif // H_WIN32

//----------------------------------------------------------------------

#ifdef H_LINUX

bool h_DetectWine(void)
{
  return false;
}

#endif // H_LINUX

//----------------------------------------------------------------------
#endif // h_DetectWine_included
