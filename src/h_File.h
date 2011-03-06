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

#include "stdio.h"
#include "src/h_Defines.h"
#include "src/h_Stdlib.h"
#include "src/h_Memory.h"
#include "src/h_BasePath.h"
#include "src/h_Debug.h"

#define H_FILE_RB (char*)"rb"
#define H_FILE_R  (char*)"r"

#define H_FILE_WB (char*)"wb"
#define H_FILE_W  (char*)"w"
#define H_FILE_AB (char*)"ab"
#define H_FILE_A  (char*)"a"

#define H_FILE_RAUTO 0

class h_File
{
  private:
    char*           m_Mode;

  public:
    FILE*           m_File;
    char            m_FilePath[H_MAX_PATHSIZE];
    void*           m_Buffer;
    unsigned long   m_Length;
    unsigned long   m_Size;

    h_File()
      {
        m_File = H_NULL;
        m_Buffer = H_NULL;
      }

    ~h_File()
      {
        close();
        freebuf();
      }

    // close
    void close(void)
      {
        if (m_File)
          fclose(m_File);
        m_File = H_NULL;
      }

    // test
    int test(const char* a_FilePath, char* a_Mode)
      {
        m_File = fopen(a_FilePath, a_Mode);
        if (m_File)
        {
          close();
          return 1;
        }
        else
        {
          return 0;
        }
      }

    // free buffer
    void freebuf(void)
      {
        if (m_Buffer)
          h_Free(m_Buffer);
        m_Buffer = H_NULL;
      }

    // set path
    char* path(const char* a_FileName)
      {
        m_FilePath[0] = '\0';
        h_GetBasePath(m_FilePath);
        h_Strcat(m_FilePath, (char*)a_FileName);
        return m_FilePath;
      }

    // get length
    unsigned long length( const char* a_FileName,
                          char* a_Mode = H_FILE_RB)
      {
        m_Mode = a_Mode;

        path(a_FileName);
        FILE* tmp_file = fopen(m_FilePath, m_Mode);
        if (!tmp_file)
        {
          dtrace("h_File.size, #ERR open(" << m_Mode << "): " << m_FilePath);
          return 0;
        }
        fseek(tmp_file, 0, SEEK_END);
        m_Length = ftell(tmp_file);
        fseek(tmp_file, 0, SEEK_SET);
        fclose(tmp_file);

        return m_Length;
      }

    // read
    unsigned long read( const char* a_FileName,
                        void* a_Buffer,
                        const unsigned long a_Length = H_FILE_RAUTO,
                        const unsigned long a_Size = 1,
                        char* a_Mode = H_FILE_RB)
      {
        m_File    = H_NULL;
        m_Buffer  = a_Buffer;
        m_Length  = a_Length;
        m_Size    = a_Size;
        m_Mode    = a_Mode;

        path(a_FileName);
        m_File = fopen(m_FilePath, m_Mode);

        dtrace("h_File.read, open(" << m_Mode << "): " << m_FilePath);

        if (!m_File)
        {
          dtrace("h_File.read, #ERR open(" << m_Mode << "): " << m_FilePath);
          return 0;
        }

        if (a_Length == H_FILE_RAUTO)
        {
          m_Length = length(a_FileName);

          if (!m_Length)
          {
            dtrace("h_File.read, #ERR null sized: " << m_FilePath);
            return 0;
          }
          a_Buffer = m_Buffer = h_Malloc(m_Length);
        }

        unsigned long result = fread(m_Buffer, m_Size, m_Length, m_File);

        if (!result)
        {
          dtrace("h_File.read, #ERR read: " << m_FilePath);
          return 0;
        }
        close();

        return result;
      }

    // write
    unsigned int write (const char* a_FileName,
                        void* a_Buffer,
                        const unsigned long a_Length = 0,
                        const unsigned long a_Size = 1,
                        char* a_Mode = H_FILE_WB)
      {
        if (a_Buffer == H_NULL)
        {
          dtrace("h_File.write, #ERR no buffer for: "<< m_FilePath);
          return 0;
        }

        m_File    = H_NULL;
        m_Buffer  = a_Buffer;
        m_Length  = a_Length;
        m_Size    = a_Size;
        m_Mode    = a_Mode;

        path(a_FileName);
        m_File = fopen(m_FilePath, m_Mode);
        dtrace("h_File.write, open(" << m_Mode << "): " << m_FilePath);

        if (!m_File)
        {
          dtrace("h_File.write, #ERR open(" << m_Mode << "): " << m_FilePath);
          return 0;
        }

        if (!m_Length)
        {
          dtrace("h_File.write, #ERR write 0 bytes: " << m_FilePath);
          return 0;
        }

        if (!fwrite(m_Buffer, m_Size, m_Length, m_File))
        {
          dtrace("h_File.write, #ERR write: " << m_FilePath);
          return 0;
        }
        close();

        return 1;
      }
};

//----------------------------------------------------------------------
#endif // h_File_included
























/*
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
*/

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
