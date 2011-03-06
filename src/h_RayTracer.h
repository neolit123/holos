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
#ifndef h_RayTracer_included
#define h_RayTracer_included
//----------------------------------------------------------------------

#include "src/h_Vector.h"

// 32 bit, rgba
typedef unsigned long rt_col;

//----------------------------------------------------------------------
// base classes
//----------------------------------------------------------------------

class rt_Ray
{
  private:
    h_Vector    m_Origin;
    h_Vector    m_Direction;

  public:

    rt_Ray()
      {
        m_Origin.set(0,0,0);
        m_Direction.set(0,0,0);
      }

    rt_Ray(h_Vector a_Origin, h_Vector a_Direction)
      {
        m_Origin = a_Origin;
        m_Direction = a_Direction;
      }

    ~rt_Ray()
      {
      }

};

//----------------------------------------------------------------------

class rt_Object
{
  public:
    float intersect(rt_Ray ray) { return 0; }
    bool  intersect(rt_Ray ray, float* t0, float* t1) { return false; }
};

typedef h_Array<rt_Object*> rt_Objects;

//----------------------------------------------------------------------

class rt_Scene
{
  private:
    rt_Objects  m_Objects;
};

//----------------------------------------------------------------------
// objects
//----------------------------------------------------------------------

class rt_Sphere : public rt_Object
{
  private:
    h_Vector  m_Pos;
    float     m_Radius;
};

//----------------------------------------------------------------------
// main raytracer
//----------------------------------------------------------------------

class h_RayTracer
{
  public:

    // recursively trace a ray

    rt_col traceray(rt_Ray ray)
      {
        return 0;
      }

    // render a screen pixel

    rt_col render(float sx, float sy)
      {
        h_Vector orig(0,0,-5);
        h_Vector dir(sx,sy,0);
        dir.sub(orig);
        dir.normalize();
        rt_Ray ray(orig,dir);
        return traceray(ray);
      }


};

//----------------------------------------------------------------------
#endif

