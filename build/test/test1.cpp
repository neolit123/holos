//#define H_DEBUG_LOG "holos_win.log"
//#define H_DEBUG_CON
//#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE
//#define H_AUTOSYNC

#include "holos.h"

//#include "lib/h_BasePath.h"
//#include "lib/h_Rand.h"
//#include "lib/h_Expression.h"

#ifndef H_NOGUI
  #include "gui/h_Window.h"
#endif

//#include "core/par/par_Float.h"

//----------------------------------------------------------------------

#ifndef H_NOGUI

class my_Widget : public h_Widget
{
  public:

    my_Widget(h_WidgetListener* a_Listener, h_Rect a_Rect, int a_Align/*=wa_None*/)
    : h_Widget(a_Listener,a_Rect,a_Align)
      {
      }
    virtual ~my_Widget()
      {
      }
    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect)
      {
        a_Painter->setFillColor( H_RGB(128,128,128) );
        a_Painter->fillRect(m_Rect.x, m_Rect.y, m_Rect.x2(), m_Rect.y2());
        a_Painter->setTextColor( H_RGB(255,255,255) );
        a_Painter->drawText(10,10,(char*)"hello world!");
      }

};

#endif // H_NOGUI

//----------------------------------------------------------------------

h_ParamDescr my_Params[] =
{
  {"param1","",pt_None,    PF_DEFAULT, 0,   /*0,1,0*/},
  {"param2","",pt_None,    PF_DEFAULT, 0.5,   /*0,1,0*/},
  {"param3","",pt_None,    PF_DEFAULT, 1,   /*0,1,0*/}
//  {"param2","",pt_Float,   PF_DEFAULT, 0.5, 0,1,0 },
//  {"param3","",pt_FloatPow,PF_DEFAULT, 1.0, 0,1,0, 2}
};

//----------------------------------------------------------------------

h_Descriptor my_Descriptor =
{
  "build.cpp",
  "ccernn",
  "holos test plugin",
  0,
  H_MAGIC,
  df_HasEditor | df_ReceiveMidi,//df_None,
  2,    // inputs
  2,    // outputs
  3,    // parameters
  0,    // programs
  my_Params,
  H_NULL,
  h_Rect(512,256)
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

class my_Instance : public h_Instance,
                    public h_WidgetListener
{
  private:
    h_Window*   m_Window;
    my_Widget*  m_Widget;
  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        m_Window = H_NULL;
        m_Widget = H_NULL;
      }

    //virtual ~my_Instance()
    //  {
    //  }

    //----------

    virtual void do_HandleState(int a_State)
      {
        switch(a_State)
        {
          case is_Resume:
            prepareParameters();
            break;
        }
      }

    //----------

    //virtual void do_HandleParameter(h_Parameter* a_Parameter)
    //  {
    //  }

    //virtual void do_HandleMidi(int a_Offset, unsigned char a_Msg1, unsigned char a_Msg2, unsigned char a_Msg3)
    //  {
    //  }

    //virtual bool do_ProcessBlock(float** a_Inputs, float** a_Outputs, int a_Length)
    //  {
    //    return false;
    //  }

    //virtual void do_ProcessSample(float** a_Inputs, float** a_Outputs)
    //  {
    //  }

    //virtual void do_PostProcess(float** a_Inputs, float** a_Outputs, int a_Length)
    //  {
    //  }

  #ifndef H_NOGUI

    virtual void* do_OpenEditor(void* ptr)
      {
        h_Rect rect = getEditorRect();
        m_Window = new h_Window(this,rect,ptr);
        m_Widget = new my_Widget(this,h_Rect(100,100),wa_Client);
        m_Window->appendWidget(m_Widget);
        m_Window->do_Realign();
        m_Window->show();
        return (void*)m_Window;
      }

    //----------

    virtual void do_CloseEditor(void)
      {
        m_Window->hide();
        delete m_Window;
        m_Window = H_NULL;
      }

    //virtual void do_IdleEditor(void)
    //  {
    //  }

  #endif // H_NOGUI

};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"


