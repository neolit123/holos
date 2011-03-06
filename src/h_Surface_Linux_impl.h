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
#ifndef h_Surface_Linux_impl_included
#define h_Surface_Linux_impl_included
//----------------------------------------------------------------------
// Pixmap

#include <X11/Xlib.h>

#ifdef H_ALPHA
#include <X11/extensions/Xrender.h>
#endif

//#include "gui/h_Painter.h"
#include "src/h_Painter.h"

//----------------------------------------------------------------------

class h_Surface_Linux : public h_PaintSource
{
  private:
    Display*    m_Display;
    Drawable    m_Drawable;
    Pixmap      m_Pixmap;
    int         m_Width;
    int         m_Height;
    int         m_Depth;
    h_Painter*  m_Painter;
    #ifdef H_ALPHA
    Picture     m_Picture;
    #endif
    //#ifdef H_OPENGL
    //#endif

  public:

    // accessors
    //inline Pixmap     getPixmap(void)  { return m_Pixmap; }
    inline h_Painter* getPainter(void) { return m_Painter; }
    // h_PaintSource:
    virtual Drawable getSourceDrawable(void) { return m_Pixmap; }
    #ifdef H_ALPHA
    virtual Picture  getSourcePicture(void)  { return m_Picture; }
    #endif
    // h_PaintTarget:
    //virtual Drawable getTargetDrawable(void) { return m_Pixmap; }

  public:

    //h_Surface(h_PaintTarget* a_PaintTarget, int a_Width, int a_Height, int a_Depth)
    h_Surface_Linux(Display* a_Display, Drawable a_Drawable, int a_Width, int a_Height, int a_Depth)
      {
        m_Display   = a_Display;
        m_Drawable  = a_Drawable;//XDefaultRootWindow(m_Display);

        //if (!m_Drawable)
        //m_Drawable = XDefaultRootWindow(m_Display);

        m_Width     = a_Width;
        m_Height    = a_Height;
        m_Depth     = a_Depth;

        // The server uses the specified drawable to determine on which screen
        // to create the pixmap. The pixmap can be used only on this screen and
        // only with other drawables of the same depth (see XCopyPlane for an
        // exception to this rule).

        m_Pixmap    = XCreatePixmap(m_Display,m_Drawable,m_Width,m_Height,m_Depth);
        m_Painter   = new h_Painter(m_Display,m_Pixmap);

        #ifdef H_ALPHA
          XRenderPictFormat* fmt;
          if (a_Depth==24) fmt = XRenderFindStandardFormat(m_Display,PictStandardRGB24);
          else fmt = XRenderFindStandardFormat(m_Display,PictStandardARGB32);
          XRenderPictureAttributes pict_attr;
          pict_attr.poly_edge = PolyEdgeSmooth;
          pict_attr.poly_mode = PolyModeImprecise;
          //pict_attr.component_alpha = true;
          int pict_bits = /*CPComponentAlpha |*/ CPPolyEdge | CPPolyMode;
          m_Picture = XRenderCreatePicture(m_Display,/*mDrawable*/m_Pixmap,fmt,pict_bits,&pict_attr);
          m_Painter->setPicture(m_Picture);
        #endif

        //#ifdef H_OPENGL
        //  //GLXPixmap glXCreatePixmap(Display* dpy, GLXFBConfig config, Pixmap pixmap, NULL);
        //#endif
      }

    ~h_Surface_Linux()
      {
        //#ifdef H_OPENGL
        //#endif
        //#ifdef H_ALPHA
        //  XRenderFreePicture(m_Display,m_Picture);
        //#endif

        delete m_Painter;

        // The XFreePixmap() function first deletes the association between the
        // pixmap ID and the pixmap. Then, the X server frees the pixmap storage
        // when there are no references to it.

        XFreePixmap(m_Display,m_Pixmap);

      }
};

//----------------------------------------------------------------------

typedef h_Surface_Linux h_Surface_Impl;

//----------------------------------------------------------------------
#endif
