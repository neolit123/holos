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
#ifndef h_Renderer_included
#define h_Renderer_included
//----------------------------------------------------------------------

class h_RenderSource
{
  public:
    virtual void getHandle(void) = 0;//{}
};

//----------

class h_RenderTarget
{
  public:
    virtual void getHandle(void) = 0;//{}
};

//----------------------------------------------------------------------

class h_Renderer
{
};

//----------------------------------------------------------------------
#endif

