#ifndef h_Texture_included
#define h_Texture_included
//----------------------------------------------------------------------
// pbuffer

#include <GL/gl.h>
#include <GL/glx.h>

#include "h_Renderer.h"

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



