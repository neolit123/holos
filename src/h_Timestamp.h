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

#ifndef h_Timestamp_included
#define h_Timestamp_included
//----------------------------------------------------------------------

#include "time.h"
#include "src/h_Stdlib.h"

class h_Timestamp
{
  private:
    time_t t_raw;

  public:
    h_Timestamp()   {}
    ~h_Timestamp()  {}

    time_t  raw(void) { return t_raw = time(0); }
    void*   ptr(void) { return &t_raw; }
    char*   str(void)
    {
      char t_str[30];
      t_raw = time(0);
      return h_Itoa(t_str, t_raw);
    }
};

#endif // h_Timestamp_included
