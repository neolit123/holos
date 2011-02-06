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
#ifndef h_File_included
#define h_File_included
//----------------------------------------------------------------------

class h_File
{
  public:
    h_File(char* filename, int mode) {}
    ~h_File() {}
    bool open() { return false; }
    void close(void) {}
    int  size(char) { return 0;; }
    int  read(char* buffer, int num) { return 0; }
    int  write(char* buffer, int num) { return 0; }
};

//// read file from base path
//
//__axstdlib_inline
//unsigned char* axFileRead (const char* _file, unsigned int* _size, const unsigned int mode = 0)
//{
//  char filepath[AX_MAX_PATH] = "";
//  char _path[AX_MAX_PATH] = "";
//  const char* path = axGetBasePath(_path);
//  axStrcat(filepath, (char*)path);
//  axStrcat(filepath, (char*)_file);
//
//  trace("axFileRead(" << mode << "): " << filepath);
//  FILE* f = NULL;
//  switch (mode)
//  {
//    case 0: f = fopen(filepath, "rb"); break;
//    case 1: f = fopen(filepath, "r");  break;
//  }
//  if (!f)
//  {
//    trace("axFileRead, #ERR open(" << mode << "): " << filepath);
//    return 0;
//  }
//  fseek(f, 0, SEEK_END);
//  *_size = ftell(f);
//  fseek(f, 0, SEEK_SET);
//  if (!*_size)
//  {
//    trace("axFileRead, #ERR null sized: " << filepath);
//    return 0;
//  }
//  unsigned char* b = (unsigned char*)axMalloc(*_size);
//  unsigned int res = fread(b, *_size, 1, f);
//  fclose(f);
//  if (!res)
//  {
//    trace("axFileRead, #ERR read: " << filepath);
//    return 0;
//  }
//  return b;
//}
//
////----------
//
//// write file to base path
//
//__axstdlib_inline
//unsigned int axFileWrite (const char* _file, const char* b, const unsigned int len, const unsigned int mode = 0 )
//{
//  char filepath[AX_MAX_PATH] = "";
//  char _path[AX_MAX_PATH] = "";
//  const char* path = axGetBasePath(_path);
//  axStrcat(filepath, (char*)path);
//  axStrcat(filepath, (char*)_file);
//
//  trace("axFileWrite(" << mode << "): " << filepath);
//  FILE* f = NULL;
//  switch (mode)
//  {
//    case 0: f = fopen(filepath, "wb"); break;
//    case 1: f = fopen(filepath, "w");  break;
//    case 2: f = fopen(filepath, "ab"); break;
//    case 3: f = fopen(filepath, "a");  break;
//  }
//  if (!f)
//  {
//    trace("axFileWrite, #ERR open(" << mode << "): " << filepath);
//    return 0;
//  }
//  unsigned int res = fwrite(b, sizeof(b[0]), len, f);
//  fclose(f);
//  if (!res)
//  {
//    trace("axFileWrite, #ERR write: " << filepath);
//    return 0;
//  }
//  return 1;
//}
//

//----------------------------------------------------------------------
#endif
