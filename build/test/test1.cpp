//#define H_DEBUG_LOG "holos_win.log"
//#define H_DEBUG_CON
//#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE



#define H_AUTOSYNC

#include "holos.h"

//#include "lib/h_BasePath.h"
//#include "lib/h_Rand.h"
//#include "lib/h_Expression.h"

#include "lib/h_Library.h"
#include "gui/h_Window.h"

//----------------------------------------------------------------------

class my_Widget : public h_Widget
{
  public:

    my_Widget(h_WidgetListener* a_Listener, h_Rect a_Rect)
    : h_Widget(a_Listener,a_Rect)
      {
      }
    virtual ~my_Widget()
      {
      }
    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect)
      {
        //trace("paint");
        a_Painter->setFillColor( H_RGB(128,128,128) );
        a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
        a_Painter->setTextColor( H_RGB(255,255,255) );
        a_Painter->drawText(10,10,"hello world!");
      }

};

//----------------------------------------------------------------------

h_ParamDescr my_Params[] =
{
  {"param1","",pt_None,PF_DEFAULT, 0,   /*0,1,0*/},
  {"param2","",pt_None,PF_DEFAULT, 0.5, /*0,1,0*/},
  {"param3","",pt_None,PF_DEFAULT, 1.0, /*0,1,0*/}
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
        //m_EditorRect = a_Descriptor->m_EditorRect;
      }

    virtual ~my_Instance()
      {
        //if (m_Window) delete m_Window;
      }

    //----------

    virtual void do_HandleState(int a_State)
      {
        const char* text = "?";
        switch(a_State)
        {
          case is_None:    text = "is_None";    break;
          case is_Open:    text = "is_Open";    break;
          case is_Close:   text = "is_Close";   break;
          case is_Suspend: text = "is_Suspend"; break;
          case is_Resume:
            prepareParameters();
            text = "is_Resume";
            break;
          case is_Rate:    text = "is_Rate";    break;
          case is_Block:   text = "is_Block";   break;
        }
        trace("do_HandleState: " << a_State << " = " << text);
      }

    //----------

    virtual void do_HandleTransport(int a_State)
      {
        char text[H_MAX_STRINGSIZE] = "";
        if (a_State & ft_None)        h_Strcat(text,"ft_None ");
        if (a_State & ft_Changed)     h_Strcat(text,"ft_Changed ");
        if (a_State & ft_Playing)     h_Strcat(text,"ft_Playing ");
        if (a_State & ft_CycleActive) h_Strcat(text,"ft_CycleActive ");
        if (a_State & ft_Recording)   h_Strcat(text,"ft_Recording ");
        if (a_State & ft_AutoWriting) h_Strcat(text,"ft_AutoWriting ");
        if (a_State & ft_AutoReading) h_Strcat(text,"ft_AutoReading ");
        trace("do_HandleTransport: " << a_State << " = " << text);
      }

    //----------

    virtual void do_HandleParameter(h_Parameter* a_Parameter)
      {
        char name[H_MAX_STRINGSIZE] = "";
        a_Parameter->do_GetName(name);
        trace("do_HandleParameter: " << a_Parameter->getIndex() << " (" << name << ") =" << a_Parameter->getValue());
      }

    //----------

    virtual void do_HandleMidi(int a_Offset, unsigned char a_Msg1, unsigned char a_Msg2, unsigned char a_Msg3)
      {
        trace("do_HandleMidi: " << a_Offset << ", " << (int)a_Msg1 << "," << (int)a_Msg2 << "," << (int)a_Msg3);
      }

    //----------

    virtual bool do_ProcessBlock(float** a_Inputs, float** a_Outputs, int a_Length) { return false; }
    virtual void do_ProcessSample(float** a_Inputs, float** a_Outputs) {}
    virtual void do_PostProcess(float** a_Inputs, float** a_Outputs, int a_Length) {}

    virtual void* do_OpenEditor(void* ptr)
      {
        h_Rect rect = getEditorRect();
        m_Window = new h_Window(this,rect,ptr);
        m_Widget = new my_Widget(this,h_Rect(100,100));
        m_Window->appendWidget(m_Widget);
        m_Window->show();
        return (void*)m_Window;
      }

    virtual void do_CloseEditor(void)
      {
        m_Window->hide();
        delete m_Window;
        //delete m_Widget;
      }

    //virtual void do_IdleEditor(void) {}

};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"


