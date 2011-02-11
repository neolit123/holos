#ifndef h_Renderer_included
#define h_Renderer_included
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
