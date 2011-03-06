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
#ifndef h_BasePath_included
#define h_BasePath_included
//----------------------------------------------------------------------

#include "stdio.h"
#include "stdlib.h"
#include "src/h_Stdlib.h"

//----------------------------------------------------------------------

char* h_GetHomePath(char* a_Buffer)
{
  //h_Strcat(a_Buffer, getenv("HOME"));
  a_Buffer[0] = '\0';
  char* env = getenv("HOME");
  if (env) h_Strcat(a_Buffer,env);
  //h_Strcat(a_Buffer, (char*)"\\");
  h_Strcat(a_Buffer, (char*)"/");
  return a_Buffer;
}

//----------------------------------------------------------------------
#ifdef H_WIN32

  #include <windows.h>
  #include "src/h_Globals.h"

  char* h_GetBasePath(char* a_Buffer)
  {
    #if defined H_LIB || defined H_EXE
      char filepath[H_MAX_PATHSIZE] = "";
      GetModuleFileName(static_WinInstance, filepath, H_MAX_PATHSIZE-1);
      filepath[H_MAX_PATHSIZE-1] = '\0';
      const char* slash = h_Strrchr(filepath,'\\');

      if (slash)
      {
        int len = (slash+1) - (char*)filepath;
        h_Strncpy(a_Buffer, filepath, len);
        a_Buffer[len] = '\0';
      }
      else
      {
        h_Strcat(a_Buffer,(char*)".\\");
      }
    #else
      h_Strcat(a_Buffer,(char*)".\\");
    #endif
    return a_Buffer;
  }

#endif
//----------------------------------------------------------------------
#ifdef H_LINUX

  #include <dlfcn.h>
  #include <unistd.h>

  const char* h_GetBasePath(char* a_Buffer)
  {
    #if defined H_LIB
      Dl_info dli;
      dladdr(__func__, &dli);
      const char* slash = h_Strrchr(dli.dli_fname, '/');
      if (slash)
      {
        int len = (slash + 1) - (char*)dli.dli_fname;
        h_Strncpy(a_Buffer, dli.dli_fname, len);
        a_Buffer[len] = '\0';
      }
      else
      {
        h_Strcat(a_Buffer, (char*)"./");
      }
    #elif defined H_EXE
      char filepath[H_MAX_PATHSIZE] = "";
      long rd_res = readlink("/proc/self/exe", filepath, H_MAX_PATHSIZE);
      filepath[H_MAX_PATHSIZE-1] = '\0';
      if (rd_res)
      {
        const char* slash = h_Strrchr(filepath, '/');
        if (slash)
        {
          int len = (slash + 1) - (char*)filepath;
          h_Strncpy(a_Buffer, filepath, len);
          a_Buffer[len] = '\0';
        }
        else
        {
          h_Strcat(a_Buffer, (char*)"./");
        }
      }
    #else
      h_Strcat(a_Buffer, (char*)"./");
    #endif
    return a_Buffer;
  }

#endif
//----------------------------------------------------------------------
#endif
