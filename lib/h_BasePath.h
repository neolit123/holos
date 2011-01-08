#ifndef h_BasePath_included
#define h_BasePath_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
#ifdef H_WIN32

  #include <windows.h>
  #include "lib/h_Globals.h"  // static_WinInstance
  #include "lib/h_Stdlib.h"     // h_Strrchr, h_Strncpy, h_Strcat

  char* h_GetBasePath(char* a_Buffer)
  {
    #if defined H_LIB || defined H_EXE
      char filepath[H_MAX_PATHSIZE] = "";
      GetModuleFileName((HINSTANCE__*)static_WinInstance,filepath,H_MAX_PATHSIZE-1);
      filepath[H_MAX_PATHSIZE-1] = '\0'; // just in case??
      const char* slash = h_Strrchr(filepath,'\\');
      if (slash)
      {
        int len = (slash+1) - (char*)filepath;
        h_Strncpy(a_Buffer,filepath,len); // (slash + 1) - (char*)filepath
        a_Buffer[len] = 0;
      }
      else h_Strcat(a_Buffer,(char*)".\\");
    #else
      h_Strcat(a_Buffer,(char*)".\\");
    #endif
    return a_Buffer;
  }

#endif
//----------------------------------------------------------------------
#ifdef H_LINUX


#endif
//----------------------------------------------------------------------
#endif
