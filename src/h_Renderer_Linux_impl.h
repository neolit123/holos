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
#ifndef h_Renderer_Linux_impl_included
#define h_Renderer_Linux_impl_included
//----------------------------------------------------------------------

#include <GL/gl.h>
#include <GL/glx.h>

//----------------------------------------------------------------------

/*

- a canvas we can render WITH
- (use as texture, blit?)
- h_Texture [h_Surface, h_Window ?]

*/

// h_Texture
class h_RenderSource
{
  public:
    virtual GLXDrawable   getSourceGLXDrawable(void) { return NULL; } // NULL
    virtual GLXFBConfig*  getSourceFbConfig(void) { return NULL; }
};

//----------

/*

- a canvas we can render TO
- h_Renderer.setRenderTarget()
- h_Texture [h_Surface, h_Window ?]

*/

// h_Texture, h_Surface, h_Window
class h_RenderTarget
{
  public:
    virtual GLXDrawable getTargetGLXDrawable(void) { return NULL; } // NULL

};

//----------------------------------------------------------------------

int h_fbconfig_attr[] =
{
  GLX_BUFFER_SIZE,      32,
  GLX_DOUBLEBUFFER,     False,
  GLX_RED_SIZE,         8,
  GLX_GREEN_SIZE,       8,
  GLX_BLUE_SIZE,        8,
  GLX_ALPHA_SIZE,       8,
  GLX_RENDER_TYPE,      GLX_RGBA_BIT,     // default
  //GLX_DRAWABLE_TYPE,    GLX_PBUFFER_BIT,  // GLX_WINDOW_BIT GLX_PIXMAP_BIT GLX_PBUFFER_BIT
  GLX_X_RENDERABLE,     True,
  None
};

//----------------------------------------------------------------------

class h_Renderer
{
  private:
    Display*      m_Display;
    GLXDrawable   m_GLXDrawable;
    GLXFBConfig*  m_fbConfig;
    XVisualInfo*  m_fbVisual;
    GLXContext    m_RC;

  //public:
  //  inline Display*     getDisplay(void)  { return m_Display; }
  //  inline GLXFBConfig* getFbConfig(void) { return m_fbConfig; }    // screen
  //  inline XVisualInfo* getFbVisual(void) { return m_fbVisual; }    // screen

  public:

    h_Renderer(h_RenderTarget* a_RenderTarget)
      {
        int numcfg;
        m_Display     = H_INTERFACE->getDisplay();
        m_GLXDrawable = a_RenderTarget->getTargetGLXDrawable();
        int screen    = H_INTERFACE->getScreen();
        m_fbConfig    = glXChooseFBConfig(m_Display,screen,h_fbconfig_attr,&numcfg);
        m_fbVisual    = glXGetVisualFromFBConfig(m_Display,m_fbConfig[0]); // use first (0)
        m_RC          = glXCreateContext(m_Display,m_fbVisual,NULL,True);
      }

    ~h_Renderer()
      {
        // todo: cleanup
      }

    //----------

    /*virtual*/
    void setRenderTarget(h_RenderTarget* a_RenderTarget)
      {
        glXMakeCurrent(m_Display,a_RenderTarget->getTargetGLXDrawable(),m_RC);
        setRenderTarget(a_RenderTarget);
      }

    //TODO: resetRenderTarget?

    //----------

    //void clear(int r, int g, int b, int a)
    //  {
    //  }

};

//----------------------------------------------------------------------
#endif
