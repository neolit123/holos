
//#define H_DEBUG_CON
//#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE

//#define H_DEBUG_LOG "holos_debug.log"

//#define H_DEBUG_MEM
//#define H_DEBUG_MEM_PRINT
//#define H_DEBUG_MEM_NOREDEFINE
//#define H_DEBUG_NEW

#include "holos.h"
#include "src/h_SkinDefault.h"
#include "src/h_WdgBackground.h"
#include "src/h_WdgPanel.h"
#include "src/h_WdgLabel.h"
#include "src/h_WdgButton.h"
#include "src/h_WdgValue.h"
#include "src/h_WdgSlider.h"
#include "src/h_WdgImage.h"
#include "src/h_WdgKnob.h"

#include "src/h_BasePath.h"
#include "src/h_BitmapLoader.h"
#include "src/h_Bitmap.h"
#include "src/h_Surface.h"

//----------------------------------------------------------------------

//class my_Widget : public h_Widget
//{
//  public:
//    h_Bitmap*  m_Bitmap;
//    h_Surface* m_Surface;
//
//  public:
//
//    my_Widget(h_WidgetListener* a_Listener, h_Rect a_Rect, int a_Align/*=wa_None*/)
//    : h_Widget(a_Listener,a_Rect,a_Align)
//      {
//      }
//
//    //virtual ~my_Widget()
//    //  {
//    //  }
//
//    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
//      {
//        int x1 = m_Rect.x;
//        int y1 = m_Rect.y;
//        int x2 = m_Rect.x2();
//        int y2 = m_Rect.y2();
//        a_Painter->setDrawColor( H_RGB(160,160,160) );
//        a_Painter->setPenWidth(5);
//        a_Painter->drawArc(x1,y1,x2,y2,0,0.75);
//        a_Painter->resetPen();
//
//        a_Painter->drawBitmap( m_Bitmap,   0,0,  0,0,456,108 );
//        a_Painter->drawSurface( m_Surface, 10,120, 0,0,64,64 );
//
//      }
//
//};

//----------------------------------------------------------------------

const char* strings[] = { (char*)"string0", (char*)"string1", (char*)"string2", (char*)"string3" };

float p2(const float x) { return x*x; }
float inv_p2(const float x) { return h_Sqrt(x); }

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

        appendParameter( new h_Parameter("param",   "", PF_DEFAULT, 0 ) );
        appendParameter( new h_ParFloat( "float",   "", PF_DEFAULT, 1.5, 0,5, 0.25 ) );
        appendParameter( new h_ParInt(   "int",     "", PF_DEFAULT, 3,   1,   10 ) );

        appendParameter( new h_ParFloat("parFloat", "", PF_DEFAULT, 440, 1, 44100, 0, &p2, &inv_p2 )); // 16
        appendParameter( new h_ParInt(  "parInt",   "", PF_DEFAULT, 256, 1,1024, &p2, &inv_p2 )); // 16
        appendParameter( new h_ParText( "parText",  "", PF_DEFAULT, 2,   4, strings));
      }
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

class my_Instance : public h_Instance,
                    public h_WidgetListener
{
  private:
    h_Editor*       m_Editor;
    h_Skin*         m_Skin;
    h_Surface*      srf;
    h_BitmapLoader* loader;

  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        m_Editor = H_NULL;
        m_Skin   = H_NULL;
        srf      = H_NULL;
        // do not call virtual methods in the constructor.
        // this one might call other virtual functions, like do_HandleParameter !!
        // todo: call it in do_HandleState, case is_Open:
        transferParameters();
      }

    //----------

    virtual ~my_Instance()
      {
      }

    //----------

    virtual void  do_HandleParameter(h_Parameter* a_Parameter)
      {
        char disp[H_MAX_STRINGSIZE];
        a_Parameter->getDisplay(disp);
        trace("param: " << a_Parameter->getName().ptr() << " = " << disp );
//        float i = a_Parameter->getInternal();
//        float v = a_Parameter->getValue();
//        char buf[32];
//        h_Ftoa(buf,i);
//        trace( a_Parameter->getName().ptr() );
//        trace( "  i: " << i);
//        trace( "  v: " << v);
//        a_Parameter->getDisplay(buf);
//        trace( "  d: " << buf);
      }

    //----------

    virtual void* do_OpenEditor(void* ptr)
      {

        h_Rect rect = getEditorRect();
        m_Editor = new h_Editor(this,rect,ptr);

        loader = new h_BitmapLoader((char*)"../extern/mverb/background.png"); // relative to this exe/dll
        trace("loaded ok");

        int   w = loader->getWidth();
        int   h = loader->getHeight();
        int   d = 24;//loader->getDepth();
        char* b = loader->getBuffer();

        h_Bitmap* bmp = m_Editor->createBitmap(w,h,d,b);
        bmp->prepare();
        bmp->swapRgba();
        srf = m_Editor->createSurface(w,h,d);
        srf->getPainter()->drawBitmap(bmp,0,0,0,0,w,h);
        delete bmp;

        m_Editor->setBorders(10,10,5,5);
        m_Skin = new skin_Default(m_Editor);
        m_Editor->applySkin(m_Skin);
        m_Editor->appendWidget( new h_WdgBackground(this) );

        h_Widget* panel;

        m_Editor->appendWidget(         new h_WdgPanel(m_Editor,h_Rect(128, 0), wa_Left ));
        m_Editor->appendWidget(         new h_WdgImage(m_Editor,h_Rect(  w, h), wa_Top, srf ));
        m_Editor->appendWidget(         new h_WdgPanel(m_Editor,h_Rect(  0,64), wa_Bottom ));
        m_Editor->appendWidget(         new h_WdgPanel(m_Editor,h_Rect( 64, 0), wa_Right ));
        m_Editor->appendWidget( panel = new h_WdgPanel(m_Editor,H_NULL_RECT,    wa_Client ));

          panel->setBorders(15,10,20,5);
          panel->setFlag(wf_Clipping);

          //#define WAL wa_StackedVert
          #define WAL wa_Top
          //my_Widget* mywidget;
          //panel->appendWidget( mywidget = new my_Widget( panel, h_Rect(100,20),WAL) );
          //mywidget->m_Bitmap = bmp;
          //mywidget->m_Surface = srf;

          panel->appendWidget( new h_WdgLabel( panel, h_Rect(100,20),WAL, "label") );
          panel->appendWidget( new h_WdgButton(panel, h_Rect(100,20),WAL, "button") );
          panel->appendWidget( new h_WdgValue( panel, h_Rect(100,20),WAL, 0.3) );
          panel->appendWidget( new h_WdgSlider(panel, h_Rect(100,20),WAL, 0.3) );
          panel->appendWidget( new h_WdgKnob(  panel, h_Rect(100,30),WAL, 0.3) );

          h_Widget* widget;
          panel->appendWidget( widget = new h_WdgSlider(panel, h_Rect(100,20),WAL) );
          m_Editor->connect( m_Parameters->item(2), widget );
          panel->appendWidget( widget = new h_WdgSlider(panel, h_Rect(100,20),WAL) );
          m_Editor->connect( m_Parameters->item(3), widget );
          panel->appendWidget( widget = new h_WdgSlider(panel, h_Rect(100,20),WAL) );
          m_Editor->connect( m_Parameters->item(4), widget );
          //panel->appendWidget( widget = new h_WdgSlider(panel, h_Rect(100,20),WAL) );
          panel->appendWidget( widget = new h_WdgValue(panel, h_Rect(100,20),WAL) );
          m_Editor->connect( m_Parameters->item(5), widget );

        m_Editor->do_SetSize(rect.w,rect.h); // realigns/paints buffer
        m_Editor->show();
        return (void*)m_Editor;

        #undef WAL
      }

    //----------

    virtual void do_CloseEditor(void)
      {
        m_Editor->hide();
        delete srf;
        delete m_Skin;
        delete m_Editor;
        m_Editor = H_NULL;
        // widgets are automatically deleted
      }

    //----------

    //virtual void do_IdleEditor(void)
    //  {
    //  }

    //----------

    //virtual void do_HandleState(int a_State)
    //  {
    //    switch(a_State)
    //    {
    //      case is_Resume:
    //        transferParameters();
    //        break;
    //    }
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


