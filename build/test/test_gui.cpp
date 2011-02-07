
//#define H_DEBUG_LOG "holos_win.log"
//#define H_DEBUG_CON
//#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE

#include "holos.h"

//#include "lib/h_BasePath.h"
//#include "lib/h_Rand.h"
//#include "lib/h_Expression.h"

#include "gui/h_Window.h"
#include "gui/skin/skin_Default.h"

#include "gui/wdg/wdg_Background.h"
#include "gui/wdg/wdg_Panel.h"
#include "gui/wdg/wdg_Label.h"
#include "gui/wdg/wdg_Button.h"
#include "gui/wdg/wdg_Slider.h"

//#include "core/par/par_Float.h"

//----------------------------------------------------------------------

class my_Widget : public h_Widget
{
  public:

    my_Widget(h_WidgetListener* a_Listener, h_Rect a_Rect, int a_Align/*=wa_None*/)
    : h_Widget(a_Listener,a_Rect,a_Align)
      {
      }

    //virtual ~my_Widget()
    //  {
    //  }

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        int x1 = m_Rect.x;
        int y1 = m_Rect.y;
        int x2 = m_Rect.x2();
        int y2 = m_Rect.y2();
        a_Painter->setDrawColor( H_RGB(160,160,160) );
        a_Painter->setPenWidth(5);
        a_Painter->drawArc(x1,y1,x2,y2,0,0.75);
        a_Painter->resetPen();
      }

};

//----------------------------------------------------------------------

class my_Descriptor : public h_Descriptor
{
  public:

    my_Descriptor()
    : h_Descriptor()
      {
        m_Name        = "build.cpp";
        m_Author      = "ccernn";
        m_Product     = "holos test plugin";
        m_Version     = 0;
        m_UniqueId    = H_MAGIC;
        m_Flags       = df_HasEditor | df_ReceiveMidi; //df_None;
        m_NumInputs   = 2;    // inputs
        m_NumOutputs  = 2; // outputs
        m_EditorRect  = h_Rect(0,0,640,480);
        m_Parameters.append( new h_Parameter("param1","",0,0) );
      }
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

class my_Instance : public h_Instance,
                    public h_WidgetListener
{
  private:
    h_Window*       m_Window;
    h_Skin*         m_Skin;
  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        m_Window = H_NULL;
        m_Skin   = H_NULL;
        prepareParameters();
      }

    //----------

    //virtual ~my_Instance()
    //  {
    //  }

    //----------

    virtual void* do_OpenEditor(void* ptr)
      {
        h_Rect rect = getEditorRect();
        trace("editor rect: " << rect.x <<","<< rect.y <<","<< rect.w <<","<< rect.h);
        m_Window = new h_Window(this,rect,ptr);
        m_Window->setBorders(10,10,5,5);
        m_Skin = new skin_Default();
        m_Window->applySkin(m_Skin);
        m_Window->appendWidget( new wdg_Background(this) );
        h_Widget* wdg;

        m_Window->appendWidget(       new wdg_Panel(m_Window,h_Rect(128, 0),wa_Left ));
        m_Window->appendWidget(       new wdg_Panel(m_Window,h_Rect(  0,64),wa_Top ));
        m_Window->appendWidget(       new wdg_Panel(m_Window,h_Rect( 64, 0),wa_Right ));
        m_Window->appendWidget( wdg = new wdg_Panel(m_Window,H_NULL_RECT,   wa_Client ));

          wdg->setBorders(20,20,10,10);
          wdg->appendWidget( new my_Widget( wdg, h_Rect(100,20),wa_StackedVert) );
          wdg->appendWidget( new wdg_Label( wdg, h_Rect(100,20),wa_StackedVert, "label") );
          wdg->appendWidget( new wdg_Button(wdg, h_Rect(100,20),wa_StackedVert, "button") );
          wdg->appendWidget( new wdg_Slider( wdg, h_Rect(100,20),wa_StackedVert, 0.3) );

        // not needed for standalone?
        // it will get a size-event and we realign there..
        // fix this!!!
        #ifdef H_LIB
          m_Window->do_Realign();
        #endif

        m_Window->show();
        //m_Window->setCursor(cu_Finger);
        return (void*)m_Window;
      }

    //----------

    virtual void do_CloseEditor(void)
      {
        m_Window->hide();
        delete m_Window;
        m_Window = H_NULL;
        delete m_Skin;
        // widgets are automatically deleted
      }

    //----------

    //virtual void do_IdleEditor(void)
    //  {
    //  }

    //----------

    virtual void do_HandleState(int a_State)
      {
        switch(a_State)
        {
          case is_Resume:
            //prepareParameters();
            break;
        }
      }

    //----------

    //virtual void do_HandleParameter(h_Parameter* a_Parameter)
    //  {
    //  }

    //----------

    //virtual void do_HandleMidi(int a_Offset, unsigned char a_Msg1, unsigned char a_Msg2, unsigned char a_Msg3)
    //  {
    //  }

    //----------

    //virtual bool do_ProcessBlock(float** a_Inputs, float** a_Outputs, int a_Length)
    //  {
    //    return false;
    //  }

    //----------

    //virtual void do_ProcessSample(float** a_Inputs, float** a_Outputs)
    //  {
    //  }

    //----------

    //virtual void do_PostProcess(float** a_Inputs, float** a_Outputs, int a_Length)
    //  {
    //  }

    //--------------------------------------------------

    //virtual void on_Change(h_Widget* a_Widget)
    //  {
    //    trace("my_Instance.on_Change");
    //  }

    //virtual void on_Redraw(h_Widget* a_Widget, int a_Mode)
    //  {
    //    trace("my_Instance.on_Redraw");
    //  }



};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"


