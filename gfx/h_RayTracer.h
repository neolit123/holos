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

// some of these could be expanded into proper /lib classes

struct h_Col3
{
  float r,g,b;
  h_Col3(float _r,float _g,float _b) { r=_r;g=_g;b=_b; }
};

struct h_Vec3
{
  float x,y,z;
};

//----------

struct rt_Ray
{
  h_Vec3  m_Origin;
  h_Vec3  m_Direction;
  //int     m_Medium;
  //float   m_Ior;
};

struct rt_Object
{
  int     m_Type;
  h_Vec3  m_Position;
  h_Vec3  m_Rotation;
};

struct rt_Light
{
  int     m_Type;
  h_Vec3  m_Position;
  h_Vec3   m_Rotation;
};

struct rt_Material
{
  int     m_Type;
  float   m_Ambient;
  float   m_Diffuse;
  float   m_Specular;
  h_Col3  m_AmbientColor;
  h_Col3  m_DiffuseColor;
  h_Col3  m_SpecularColor;
};

struct rt_Hit
{
  h_Vec3      m_Position;
  rt_Object*  m_Object;
  rt_Ray*     m_Ray;
};


//----------

class h_RayTracer
{
  public:

    h_RayTracer()
      {
      }

    ~h_RayTracer()
      {
      }

    h_Col3 raytrace(rt_Ray a_Ray)
      {
        return h_Col3(0,0,0);
      }
};

//----------------------------------------------------------------------
#endif

