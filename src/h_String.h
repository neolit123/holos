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
#ifndef h_String_included
#define h_String_included
//----------------------------------------------------------------------
// char* wrapper

// todo: h_Chars
// char-buffer (manages memory for string)

//#include <string.h>
//#include "lib/h_Stdlib.h"
#include "src/h_Stdlib.h"

//----------------------------------------------------------------------

class h_String
{
  private:
    /*const*/ char* mText;

  public:

    h_String()                            { mText = (char*)""; }
    h_String(char* txt)                   { mText = txt; }
    h_String(const char* txt)             { mText = (char*)txt; }
  //axString(axString* str)               { mText = str->mText; }

    char*    ptr()                        { return mText; }
    void     set(char* txt)               { mText = txt; }
    bool     empty()                      { return mText[0]==0; }
    int      length()                     { return h_Strlen(mText); }

    char     operator [] (unsigned int i) { return mText[i]; }

    void     operator = (h_String str)    { mText = str.mText; }
  //void     operator = (axString& str)   { mText = str.mText; }
    void     operator = (h_String* str)   { mText = str->mText; }
    void     operator = (char* txt)       { mText = txt; }
    void     operator = (const char* txt) { mText = (char*)txt; }

};

//----------------------------------------------------------------------

static h_String STR_EMPTY((char*)"");
static h_String STR_NONE((char*)"none");

//#include "lib/h_Array.h"
#include "src/h_Array.h"
typedef h_Array<h_String> h_Strings;

//----------------------------------------------------------------------
#endif
