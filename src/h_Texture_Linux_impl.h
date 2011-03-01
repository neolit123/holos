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
#ifndef h_Texture_Linux_impl_included
#define h_Texture_Linux_impl_included
//----------------------------------------------------------------------
// pbuffer

#include <GL/gl.h>
#include <GL/glx.h>

#include "src/h_Renderer.h"

int h_texture_attr[] =
{
  GLX_PBUFFER_WIDTH,  256,
  GLX_PBUFFER_HEIGHT, 256,
  //GLX_PRESERVED_CONTENTS, True,
  None
};

//----------------------------------------------------------------------

class h_Texture : public h_RenderSource,
                  public h_RenderTarget
{
  private:
    Display*     m_Display;
    GLXFBConfig* m_fbConfig;
    GLXPbuffer   m_pbuffer;
  public:
    // h_RenderSource
    virtual GLXDrawable  getSourceGLXDrawable(void) { return m_pbuffer; }
    virtual GLXFBConfig* getSourceFbConfig(void)    { return m_fbConfig; }
    // h_RenderTarget
    //virtual GLXDrawable getTargetGLXDrawable(void) { return NULL; }


  public:

    // shouldn't it be RenderTarget?
    h_Texture(h_RenderSource* a_RenderSource)
      {
        m_Display  = H_INTERFACE->getDisplay();
        m_fbConfig = a_RenderSource->getSourceFbConfig();
        m_pbuffer  = glXCreatePbuffer(m_Display,*m_fbConfig,h_texture_attr);
      }

    virtual ~h_Texture()
      {
         // delete m_pbuffer
      }
};

//----------------------------------------------------------------------
#endif



