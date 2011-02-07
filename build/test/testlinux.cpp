
//#define H_NOGUI

#include "holos.h"
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
    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        trace("paint");
        a_Painter->setFillColor( H_RGB(128,128,128) );
        a_Painter->fillRect(a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2());
        a_Painter->setTextColor( H_RGB(255,255,255) );
        a_Painter->drawText(10,10,(char*)"hello world!");
      }
};

//----------------------------------------------------------------------

h_Descriptor my_Descriptor =
{
  "testlinux",
  "ccernn",
  "holos test plugin",
  0,
  H_MAGIC,
  df_HasEditor | df_ReceiveMidi,//df_None,
  2,    // inputs
  2,    // outputs
  0,    // parameters
  0,    // programs
  H_NULL,//my_Params,
  H_NULL,
  h_Rect(256,256)
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
        trace("hello world");
        m_Window = H_NULL;
      }

    virtual ~my_Instance()
      {
        if (m_Window) delete m_Window;
      }

    virtual void* do_OpenEditor(void* ptr)
      {
        trace("do_OpenEditor: " << ptr);
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
        m_Window = H_NULL;
      }

};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"
