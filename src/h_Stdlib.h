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
#ifndef h_Stdlib_included
#define h_Stdlib_included
//----------------------------------------------------------------------

#include "src/h_Memory.h"

/*
  ======================================================================
  h_Stdlib.h
    a collection of optimized methods for memory and string manipulation
    to replace the default ones in C.

    code in open source projects like MINGW, MINIX, DJGPP and GCC used for
    reference.

    for more information remove the "h_" prefix from a method's name
    and use google to search.

  ======================================================================
  NOTE:
    all methods are tested (mingw32-gcc-tdm-4.4.1) with the examples
    from the website "cplusplus.com" and should work as expected.
    not much error checking going on, so the methods should be used properly
    (by the book), with "low level" programming precautions in mind.

  ======================================================================
  TODO:
    - slighly better error handling for some of the methods
  ======================================================================
*/

//#include "lib/h_Defines.h"
//#include "lib/h_Memory.h"
#include "src/h_Defines.h"
#include "src/h_Memory.h"

#ifdef _H_STDLIB_USE_INLINE
  #define _H_STDLIB_INLINE inline
#else
  #define _H_STDLIB_INLINE
#endif

//----------------------------------------------------------------------
//
// general
//
//----------------------------------------------------------------------

/**
  * h_IsDigit
  */
#define h_IsDigit(c)  ( ( (c) > 47 && (c) < 58 ) ? 1 : 0 )

/**
  * h_IsLetter
  */
#define h_IsLetter(c) \
( ( ((c) > 64 && (c) < 91) || ((c) > 96 && (c) < 123) ) ? 1 : 0 )

//----------------------------------------------------------------------
//
// memory
//
//----------------------------------------------------------------------

/**
 * h_Memchr
 */
_H_STDLIB_INLINE
void* h_Memchr (register const void* src, int c, unsigned int len)
{
  register const unsigned char* _src = (const unsigned char*) src;
  c &= 0xff;
  while (len--)
  {
    if (*_src == c) return (char*) _src;
    _src++;
  }
  return NULL;
}

/**
 * h_Memcmp
 */
_H_STDLIB_INLINE
unsigned int h_Memcmp (register const void* m1,
  register const void* m2, register unsigned int n)
{
  register unsigned char* s1 = (unsigned char*) m1;
  register unsigned char* s2 = (unsigned char*) m2;
  while (n--)
  {
    if (*s1 != *s2) return *s1 - *s2;
    s1++;
    s2++;
  }
  return 0;
}

/**
 * h_Memcpy
 */
_H_STDLIB_INLINE
void* h_Memcpy (register void* dest,
  register const void* src, register unsigned int len)
{
  register char* _d = (char*) dest;
  register char* _s = (char*) src;
  while (len--) *_d++ = *_s++;
  return dest;
}

/**
 * h_Memmove
 */
_H_STDLIB_INLINE
void* h_Memmove (register void* dst, register const void* src, unsigned int len)
{
  register char* _dst = (char*) dst;
  register const char* _src = (char*) src;
  if (_src < _dst && _dst < _src + len)
  {
    _src += len;
    _dst += len;
    while (len--) *--_dst = *--_src;
  }
  else
    while (len--) *_dst++ = *_src++;
  return dst;
}

/**
 * h_Memset
 */

_H_STDLIB_INLINE
void* h_Memset (register void* dest, register int val,
  register unsigned int len)
{
  register unsigned char _v = (unsigned char) val;
  register char* _d = (char*) dest;
  while (len--) *_d++ = _v;
  return dest;
}

//----------------------------------------------------------------------
//
// string
//
//----------------------------------------------------------------------

/**
 * h_Strlen
 */
_H_STDLIB_INLINE
unsigned int h_Strlen (register char* str)
{
  register unsigned int num = 0;
  while (*str++) num++;
  return num;
}

/**
 * h_Strcpy
 */
_H_STDLIB_INLINE
char* h_Strcpy (register char* dest, register const char* src)
{
  while ( (*dest++ = *src++) );
  return dest;
}

/**
 * h_Strncpy
 */
_H_STDLIB_INLINE
char* h_Strncpy (register char* dest, register const char* src, unsigned int n)
{
  while ( n-- && (*dest++ = *src++) != '\0' );
  return dest;
}

/**
 * h_Strdup
 */
_H_STDLIB_INLINE
char* h_Strdup (register char* src)
{
  register char* dst = (char*) h_Malloc(h_Strlen(src) + 1);
  if (!dst) return NULL;
  h_Strcpy (dst, src);
  return dst;
}

/**
 * h_Strchr
 */
_H_STDLIB_INLINE
char* h_Strchr (register const char* str, register const int c)
{
  while (*str && *str != c)
    str++;
  if (*str != c)
    str = NULL;
  return (char*) str;
}

/**
 * h_Strrchr
 */
_H_STDLIB_INLINE
char* h_Strrchr (register const char* s, const int c)
{
  char* p = NULL;
  while (*s++)
    if (*s == c)
      p = (char*) s;
  return p;
}

/**
 * h_Strcmp
 */
_H_STDLIB_INLINE
int h_Strcmp (register const char* s1,
  register const char* s2)
{
  while (*s1 != '\0' && *s1 == *s2)
  { s1++; s2++; }
  return (*(unsigned char *) s1) - (*(unsigned char*) s2);
}

/**
 * h_Strncmp
 */
_H_STDLIB_INLINE
int h_Strncmp (register const char* s1,
  register const char* s2, register unsigned int n)
{
  register unsigned char u1, u2;
  while (n--)
  {
    u1 = (unsigned char) *s1++;
    u2 = (unsigned char) *s2++;
    if (u1 != u2)
      return u1 - u2;
    if (u1 == '\0')
      return 0;
  }
  return 0;
}

/**
 * h_Strcat
 */
_H_STDLIB_INLINE
const char* h_Strcat (register char* s1,
  register const char* s2)
{
  while (*s1)
    s1++;
  while ((*s1++ = *s2++));
  return s1;
}

/**
 * h_Strncat
 */
_H_STDLIB_INLINE
char* h_Strncat (register char* s1, register char* s2,
  register unsigned int n)
{
  register char* s = s1;
  while (*s1)
    s1++;
  while ( n-- && (*s1++ = *s2++) )
    if (n == 0)
      *s1 = '\0';
  return s;
}

/**
 * h_Strstr
 */
_H_STDLIB_INLINE
char* h_Strstr (register const char* s1,
  register const char* s2)
{
  register unsigned int i;
  while (*s1++)
  {
    i = 0;
    while (1)
    {
      if (!s2[i])
        return (char*) s1;
      if (s2[i] != s1[i])
        break;
      i++;
    }
  }
  return (char*) NULL;
}

/**
 * h_Strspn
 */
_H_STDLIB_INLINE
unsigned int h_Strspn (register const char* s1,
  register const char* s2)
{
  register const char *_s1, *_s2;
  _s1 = s1;
  while (*_s1)
  {
    _s2 = s2;
    while (*_s2 && *_s2 != *_s1)
      _s2++;
    if (*_s2 == '\0')
      break;
    _s1++;
  }
  return _s1 - s1;
}

/**
 * h_Strcspn
 */
_H_STDLIB_INLINE
unsigned int h_Strcspn (register const char* s1, register const char* s2)
{
  register const char *_s1, *_s2;
  _s1 = s1;
  while (*_s1)
  {
    _s2 = s2;
    while (*_s2 != *_s1 && *_s2)
      _s2++;
    if (*_s2)
      break;
    _s1++;
  }
  return _s1 - s1;
}

/**
 * h_Strpbrk
 */
_H_STDLIB_INLINE
char* h_Strpbrk (register const char* s1, register const char* s2)
{
  register const char* _s1;
  while (*s1++)
  {
    _s1 = s2;
    while (*_s1 && *_s1 != *s1)
      _s1++;
    if (*_s1)
      return (char*) s1;
  }
  return (char*) NULL;
}

/**
 * h_Strtok
 */
_H_STDLIB_INLINE
char* h_Strtok (register char* str, const char* spr)
{
  register char *s1, *s2;
  // static buffer = kaboom ?
  static char* _save;
  // --
  if (str == NULL)
  {
    str = _save;
    if (str == NULL)
      return (char*) NULL;
  }
  s1 = str + h_Strspn(str, spr);
  if (*s1 == '\0')
    return (char*) (_save = NULL);
  s2 = h_Strpbrk(s1, spr);
  if (s2 != NULL)
    *s2++ = '\0';
  _save = s2;
  return s1;
}

/**
 * h_Strrev
 */
_H_STDLIB_INLINE
char* h_Strrev (register char* str)
{
  register unsigned int i = 0;
  register unsigned int j = h_Strlen(str)-1;
  while (i < j)
  {
    str[i] ^= str[j];
    str[j] ^= str[i];
    str[i] ^= str[j];
    ++i;
    --j;
  }
  return str;
}

//----------------------------------------------------------------------
//
// conversation
//
//----------------------------------------------------------------------

/*
 * h_Itoa(string, intnumber, maximumchars, base, flag)
 *
 * base: 16 = hex, 10 = dec, 2 = bin, etc.
 *
 * flag (works only for base = 10):
 * 0 - show minus sign only
 * 1 - show minus or plus sign
 * 2 - show minus sign or indent positive values with one space
 *
 * 'maximumchars' is the maximum number of characters including the
 * sign. maximum is 33, default is 10
 * for base 2, intnumber=2147483647 requires string length=33
 * for base 10 use 'maximumchars' = N to show N chars max
 * (e.g. -9999, +9999)
 * allocated memory for 'string' should be >= 'maximumchars'
 *
 */

_H_STDLIB_INLINE
char* h_Itoa( register char* _st, int n, unsigned int maxlen = 10,
              unsigned int base = 10, unsigned int fg = 0)
{
  if (!_st || maxlen > 33) return (char*)"0";
  register unsigned int v, p = 1;
  register int i = 0;
  char* st = _st;
  char _t[33];
  register char* t = _t;
  v = (unsigned int) n;
  if (base == 10)
  {
    if (n < 0)
    {
      v = -n;
      *st++ = '-';
      maxlen--;
    }
    else
    {
      if (fg == 1) { *st++ = '+'; maxlen--; }
      if (fg == 2) { *st++ = ' '; maxlen--; }
    }
  }
  while (i < (int)maxlen)
  {
    p *= 10;
    i++;
  }
  if (base == 10 && v >= (p-1))
  {
    while (maxlen--)
      *st++ = '9';
  }
  else
  {
    while (v || t == _t)
    {
      i = v % base;
      v = v / base;
      if (i < 10)
        *t++ = '0' + i;
      else
        *t++ = 'a' + i - 10;
    }
    while (t > _t)
      *st++ = *--t;
  }
  *st = 0;
  return _st;
}

/**
 * h_Atoi
 */
_H_STDLIB_INLINE
int h_Atoi (register const char* s)
{
  if (!s) return 0;
  const char digits[] = "0123456789";
  register unsigned val = 0;
  register int neg = 0;
  while (*s == ' ' || *s == '\t') s++;
  if (*s == '-')
  {
    neg = 1;
    s++;
  }
  else if (*s == '+') s++;
  while (*s)
  {
    const char *w;
    unsigned digit;
    w = h_Strchr(digits, *s);
    if (!w) break;
    digit = (w - digits);
    val = val*10 + digit;
    s++;
  }
  if (neg) return -val;
  return val;
}

/*
 * h_Ftoa(string, floatnumber, maximumchars, flag)
 *
 * flag:
 * 0 - show minus sign only
 * 1 - show minus or plus sign
 * 2 - show minus sign or indent positive values with one space
 *
 * 'maximumchars' is the maximum number of characters including the
 * sign and decimal point.
 * allocated memory for 'string' should be >= 'maximumchars'
 *
 */
_H_STDLIB_INLINE
char* h_Ftoa( register char* st, register double f,
              register int maxlen = 5, const unsigned int fg = 0)
              //, const bool e = false)
{
  if (!st) return (char*)"0";
  char* ret = st;
  register int exp = 0;
  register int z;
  int j = 0;
  if (f < 0)
  {
   *st++ = '-';
   j++;
   f = -f;
  }
  else
  {
    if (fg == 1) { *st++ = '+';  j++; }
    if (fg == 2) { *st++ = ' ';  j++; }
  }
  if (f)
  {
    while (f < 1.f)
    {
      f *= 10.f;
      exp--;
    }
    while ( (f >= 10.f && exp < maxlen) || exp < 0 )
    {
      f *= 0.1f;
      exp++;
    }
  }
  if (exp > maxlen - ( j + 1 ))
  {
    maxlen -= j;
    while (maxlen--) *st++ = '9';
  }
  else
  {
    while ( (exp > 0) && (exp <= maxlen) && j < maxlen-1 )
    {
      *st++ = '0' + (char)f;
      z = (int)f;
      f -= z;
      f *= 10.f;
      exp--;
      j++;
    }
    *st++ = '0' + (char)f;
    z = (int)f;
    f -= z;
    f *= 10.f;
    j++;
    if (j < maxlen-1)
    {
      *st++ = '.';
      j++;
      register unsigned int i = 0;
      while (j < maxlen)
      {
        *st++ = '0' + (char)f;
        z = (int)f;
        f -= z;
        f *= 10.f;
        i++;
        j++;
      }
    }
  }
  /*
  // note: exponent output is disabled. instead it writes the maximum integer.
  if (exp != 0 && e)
  {
    *st++ = 'e';
    if (exp < 0)
    {
      *st++ = '-';
      exp = -exp;
    }
    else *st++ = '+';
    register int expd10 = exp/10;
    *st++ = '0' + expd10;
    *st++ = '0' + (exp -= expd10 * 10);
  }
  */
  *st++ = 0;
  return ret;
}

/**
  * h_Atof
  */
_H_STDLIB_INLINE
float h_Atof (register char* s)
{
  if (!s) return 0.f;
  register float a = 0.f;
  register int e = 0;
  register unsigned int c;
  float _asign = 1.f;
  if ( s[0] == '-' )
  {
    _asign = -1.f;
    s++; //*s++;
  }
  while ( (c = *s++) != '\0' && h_IsDigit(c) )
    a = a*10.f + (c - '0');
  if (c == '.')
    while ( (c = *s++) != '\0' && h_IsDigit(c) )
    {
      a = a*10.f + (c - '0');
      e = e-1;
    }
  if (c == 'e' || c == 'E')
  {
    int sign = 1;
    register int i = 0;
    c = *s++;
    if (c == '+') c = *s++;
    else if (c == '-')
    {
      c = *s++;
      sign = -1;
    }
    while ( h_IsDigit(c) )
    {
      i = i*10 + (c - '0');
      c = *s++;
    }
    e += i*sign;
  }
  while (e > 0)
  {
    a *= 10.f;
    e--;
  }
  while (e < 0)
  {
    a *= 0.1f;
    e++;
  }
  return a*_asign;
}

//------------------------------------------------------------------------------
#endif









































#if 0

//#define h_Memset  memset
//#define h_Memcpy  memcpy
//
//#include <string.h>
//
//#define h_Strcat  strcat
//#define h_Strncpy strncpy
//#define h_Strrchr strrchr

//----------------------------------------------------------------------

#include "lib/h_Defines.h"

// not strictly just a stdlib replacement...
// also some other helper functions, etc...

// don't use anything from h_Debug
// don't use anything from h_Memory
//
// no includes!
// debugging, etc, is not set up yet..

//----------------------------------------------------------------------
// mem
//----------------------------------------------------------------------

/*inline*/
void h_Memset(void* dst, char c, int n)
  {
    char* d = (char*)dst;
    while (n--) *d++ = c;
  }

//----------

/*inline*/
void h_Memcpy(char* dst, char* src, int n)
  {
    char* d = (char*)dst;
    char* s = (char*)src;
    while (n--) *d++ = *s++;
  }

//----------

//int h_Memcmp(const void *b1, const void *b2, size_t n)
//{
//  const unsigned char *p1 = (const unsigned char*)b1;
//  const unsigned char *p2 = (const unsigned char*)b2;
//  if (!n) return 0;
//  for (size_t i = 0; i < n; i++)
//  {
//    if (p1[i] != p2[i]) return p1[i]-p2[i];
//  }
//  return 0;
//}

//----------

//void *h_Memmove(void *dst, const void *src, size_t count)
//{
//  void *ret = dst;
//  if (dst <= src || (char *)dst >= ((char *)src + count))
//  {
//    // Non-Overlapping Buffers
//    // copy from lower addresses to higher addresses
//    while (count--)
//    {
//      *(char*)dst = *(char*)src;
//      dst = (char*)dst + 1;
//      src = (char*)src + 1;
//    }
//  }
//  else
//  {
//    // Overlapping Buffers
//    // copy from higher addresses to lower addresses
//    dst = (char*)dst + count - 1;
//    src = (char*)src + count - 1;
//    while (count--)
//    {
//      *(char*)dst = *(char*)src;
//      dst = (char*)dst - 1;
//      src = (char*)src - 1;
//    }
//  }
//  return ret;
//}

//----------------------------------------------------------------------
// str
//----------------------------------------------------------------------

/*inline*/
unsigned int h_Strlen(register char* str)
  {
    register unsigned int num = 0;
    while (*str++) num++;
    return num;
  }

/*inline*/
char* h_Strcpy(char* dest,const char* src)
  {
    while ((*dest++=*src++));
    return dest;
  }

//----------

/*inline*/
char* h_Strncpy(char* dest, const char* src, unsigned int n)
  {
    while (n-- && (*dest++=*src++) != '\0');
    return dest;
  }

//----------

/*inline*/
const char* h_Strcat(/*register*/ char* s1, /*register*/ const char* s2)
  {
    while (*s1) s1++;
    while ((*s1++ = *s2++));
    return s1;
  }

//----------

/*inline*/
char* h_Strncat(/*register*/ char* s1, /*register*/ char* s2, /*register*/ unsigned int n)
  {
    /*register*/ char* s = s1;
    while (*s1) s1++;
    while ( n-- && (*s1++ = *s2++) ) if (n == 0) *s1 = '\0';
    return s;
  }

//----------

//_H_STDLIB_INLINE
//char* h_Strchr (register const char* str, register const int c)
//{
//  while (*str && *str != c)
//    str++;
//  if (*str != c)
//    str = NULL;
//  return (char*) str;
//}

const char *h_Strchr(const char *str, int ch)
  {
    while (*str)
    {
      if (*str == ch) return str;
      str++;
    }
    return 0;
  }

//----------

/*inline*/
char* h_Strrchr(const char* s, /*int*/char c)
  {
    char* p = NULL;
    while (*s++) if (*s == c) p = (char*)s;
    return p;
  }

//----------

//char *strdup(const char *src)
//{
//  if (!src) return 0;
//  char *dst = (char*)malloc(strlen(src)+1);
//  strcpy(dst, src);
//  return dst;
//}

//----------

int h_Strcmp(/*register*/ const char* s1, /*register*/ const char* s2)
  {
    while (*s1 != '\0' && *s1 == *s2)
    { s1++; s2++; }
    return (*(unsigned char *) s1) - (*(unsigned char*) s2);
  }

//int strncmp(const char *s1, const char *s2, size_t n)
//{
//  if (!n) return 0;
//  const unsigned char *p1 = (const unsigned char*)s1;
//  const unsigned char *p2 = (const unsigned char*)s2;
//  for (size_t i = 0; i < n; i++)
//  {
//    if (!p1[i] || p1[i] != p2[i]) return p1[i] - p2[i];
//  }
//  return 0;
//}

//----------

const char *h_Strstr(/*const*/ char *str, /*const*/ char *substr)
  {
    int str_len = h_Strlen(str);
    int substr_len = h_Strlen(substr);
    if (substr_len == 0) return str;
    if (str_len < substr_len) return 0;
    for (int i = 0; i < (int)(str_len-substr_len+1); i++)
    {
      if (!h_Strcmp(&str[i], substr)) return (const char *)(&str[i]);
    }
    return 0;
  }

// This function assumes the passed pointer points to a valid, zero-terminated string
// http://rosettacode.org/wiki/Reverse_a_string#C

void h_Strrev(char *s)
  {
    char *t = s;
    while (*t != '\0') t++;
    while (s < t)
    {
      int c = *s;
      *s++ = *--t;
      *t = c;
    }
  }

//----------
//----------
//----------

//int toupper(int c)
//{
//  if (c < 'a' || c > 'z') return c;
//  return c-0x20;
//}

//int tolower(int c)
//{
//  if (c < 'A' || c > 'Z') return c;
//  return c+0x20;
//}

//----------
//----------
//----------

//int isspace(int c)		{return ((c>=0x09 && c<=0x0D) || (c==0x20));}
//int isupper(int c)		{return (c>='A' && c<='Z');}
//int isalpha(int c)		{return (c>='A' && c<='Z') || (c>='a' && c<='z');}
//int isdigit(int c)		{return (c>='0' && c<='9');}
//int isxdigit(int c)		{return (c>='0' && c<='9') || (c>='A' && c<='F') || (c>='a' && c<='f');}
//int isalnum(int c)		{return isalpha(c) || isdigit(c);}
//int isprint(int c)		{return c>=' ';}

//----------------------------------------------------------------------
// file/path
//----------------------------------------------------------------------

/*inline*/
/*

// ##### attempt to put this in h_BasePath

const char* h_StripPath(const char* a_Path)
  {
    if (a_Path)
    {
      char *slash, *backslash;
      slash     = h_Strrchr(a_Path,'/');
      backslash = h_Strrchr(a_Path,'\\');//+1;
      if (slash) return slash + 1;
      else if (backslash) return backslash + 1; // see above
    }
    return (char*)"NULL";
  }
*/

//----------

//// ptr to (reversed) hex
//void h_CreateUniqueName(char* a_Buffer, char* a_Prefix, void* a_Ptr)
//  {
//    static char h_int2hex[17] = "0123456789abcdef";      // +'\0' = 17
//    while (*a_Prefix != '\0') *a_Buffer++ = *a_Prefix++;  // memset + update pointers
//    unsigned long iptr = (unsigned long)a_Ptr;            // todo: 64-bit (long long?)
//    while (iptr>0)
//    {
//      int j = iptr & 0x0f;            // 0..f
//      iptr >>= 4;                     // /16 (0..f)
//      *a_Buffer++ = h_int2hex[j];    // to char
//    }
//    *a_Buffer++ = '\0';
//  }

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

//int abs(int n) { return (n>0)?n:-n; }

//----------
//----------
//----------

// oops...
// might not be thread-safe..
// todo: import old rand generator from axonlib

static int static_LastFastRand = 0;

//---

void h_FastRand_Seed(unsigned int seed)
  {
    static_LastFastRand = seed;
  }

//---

int h_FastRand()
  {
    return (((static_LastFastRand = static_LastFastRand * 214013L + 2531011L) >> 16) & 0x7FFF);
  }

#endif // 0
