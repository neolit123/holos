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
#ifndef h_Vector_included
#define h_Vector_included
//----------------------------------------------------------------------

#include "src/h_Math.h"

class h_Vector
{
  public:
    float x,y,z;

  public:

    h_Vector()
      {
        set(0);
      }

    h_Vector(float n)
      {
        set(n);
      }

    h_Vector(float ax, float ay, float az)
      {
        set(ax,ay,az);
      }

    void set(float n)
      {
        x = n;
        y = n;
        z = n;
      }

    void set(float ax, float ay, float az)
      {
        x = ax;
        y = ay;
        z = az;
      }

    void set(h_Vector v)
      {
        x = v.x;
        y = v.y;
        z = v.z;
      }

    void add(h_Vector v)
      {
        x += v.x;
        y += v.y;
        z += v.z;
      }

    void sub(h_Vector v)
      {
        x -= v.x;
        y -= v.y;
        z -= v.z;
      }

    float dot(h_Vector v)
      {
        return ( x*v.x + y*v.y + z*v.z);
      }

    h_Vector cross(h_Vector v)
      {
        return h_Vector( y*v.z-v.y*z, v.x*z-x*v.z, x*v.y-v.x*y );
      }

    float length(void)
      {
        return h_Sqrt(x*x + y*y + z*z);
      }

    float distance(h_Vector v)
      {
        float a = x - v.x;
        float b = y - v.y;
        float c = z - v.z;
        return h_Sqrt(a*a + b*b + c*c);
      }

    void normalize(void)
      {
        float len = x*x + y*y + z*z;
        float invlen = h_InvSqrt(len);
        x *= invlen;
        y *= invlen;
        z *= invlen;
      }

};

//----------------------------------------------------------------------
#endif

