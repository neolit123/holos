
//#define H_DEBUG_CON
//#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE

//#define H_DEBUG_LOG "holos_debug.log"

//#define H_DEBUG_MEM
//#define H_DEBUG_MEM_PRINT
//#define H_DEBUG_NEW

#include "holos.h"
#include "h/h_SkinDefault.h"
#include "h/h_WdgBackground.h"
#include "h/h_WdgPanel.h"
#include "h/h_WdgLabel.h"
#include "h/h_WdgButton.h"
#include "h/h_WdgValue.h"
#include "h/h_WdgSlider.h"

#include "h/h_BasePath.h"
#include "h/h_BitmapLoader.h"
#include "h/h_Bitmap.h"
#include "h/h_Surface.h"

#include <stdio.h>  // fopen

//----------------------------------------------------------------------

class my_Widget : public h_Widget
{
  public:
    h_Bitmap*  m_Bitmap;
    h_Surface* m_Surface;

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

        a_Painter->drawBitmap( m_Bitmap,   0,0,  0,0,456,108 );
        a_Painter->drawSurface( m_Surface, 10,120, 0,0,64,64 );

      }

};

//----------------------------------------------------------------------

const char* strings[] = { (char*)"string0", (char*)"string1", (char*)"string2", (char*)"string3" };
float pow2(const float x) { return h_Pow(x,2.f); }
float inv_pow2(const float x) { return h_Log(x)/h_Log(2.f); }

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
        appendParameter( new h_ParFloat( "float",   "", PF_DEFAULT, 1.5, 0,5,0.25 ) );
        appendParameter( new h_ParInt(   "int",     "", PF_DEFAULT, 3, 1,10, H_NULL) );

        appendParameter( new h_ParFloat("parFloat", "", H_NULL, 4, 0, 10, 0.1, &pow2, &inv_pow2)); // 16
        appendParameter( new h_ParInt(  "parInt",   "", H_NULL, 4, 0, 40, &pow2, &inv_pow2)); // 16
        appendParameter( new h_ParText( "parText",  "", H_NULL, 2, 4, strings));

        // would it be possible (and/or sensible) to have the default, and min/max values
        // transformed through the inv_pow2 callbacks?
        // so we could set the top one to:
        // H_NULL, 16, 0, 1024, 0.1, &pow...
        // and the 16,0,1024 would be changed to 4,0,10 (by inv_pow2)..

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
    h_Bitmap* bmp;
    h_Surface* srf;
    h_BitmapLoader loader; // we need data after creating it

  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        m_Editor = H_NULL;
        m_Skin   = H_NULL;
        transferParameters();
        bmp = H_NULL;
        srf = H_NULL;
      }

    //----------

    virtual ~my_Instance()
      {
      }

    //----------

    virtual void  do_HandleParameter(h_Parameter* a_Parameter)
      {
        char buf[H_MAX_STRINGSIZE];
        a_Parameter->getDisplay(buf);
        //trace("param: " << a_Parameter->getName().ptr() << " = " << buf );
      }

    //----------

    virtual void* do_OpenEditor(void* ptr)
      {
        h_Rect rect = getEditorRect();
        m_Editor = new h_Editor(this,rect,ptr);
        //char temp[H_MAX_STRINGSIZE];
        //temp[0] = '\0';
        //h_Strcat(temp,"../extern/mverb/background.png");
        //trace("path: " << temp)
        loader.load((char*)"../extern/mverb/background.png");
        int w = loader.getWidth();
        int h = loader.getHeight();
        int d = 24;//loader.getDepth();
        char* b = (char*)loader.getBuffer();
        //trace("w:"<<w << ", h:"<<h << ", d:"<<d );
        bmp = m_Editor->createBitmap(w,h,d,b);
        bmp->prepare();

        //unsigned char* p = (unsigned char*)bmp->getBuffer();
        //for (int y=0; y<64; y++)
        //{
        //  for (int x=0; x<64; x++)
        //  {
        //    *p++ = 0;     // b
        //    *p++ = y * 4; // g
        //    *p++ = x * 4; // r
        //    *p++ = 0;
        //  }
        //}

        srf = m_Editor->createSurface(64,64,24);
        h_Painter* paint = srf->getPainter();
        paint->setFillColor( H_DARK_CYAN );
        paint->fillRect(0,0,63,63);
        paint->setDrawColor( H_CYAN );
        paint->drawCircle(0,0,63,63);

        m_Editor->setBorders(10,10,5,5);
        m_Skin = new skin_Default();
        m_Editor->applySkin(m_Skin);
        m_Editor->appendWidget( new h_WdgBackground(this) );

        h_Widget* wdg;
        h_Widget* wdg2;
        my_Widget* w2;

        m_Editor->appendWidget( new h_WdgPanel(m_Editor,h_Rect(128, 0),wa_Left ));
        m_Editor->appendWidget( new h_WdgPanel(m_Editor,h_Rect(  0,64),wa_Top ));
        m_Editor->appendWidget( new h_WdgPanel(m_Editor,h_Rect( 64, 0),wa_Right ));
        m_Editor->appendWidget( wdg = new h_WdgPanel(m_Editor,H_NULL_RECT,   wa_Client ));
        wdg->setBorders(20,20,10,10);

        //#define WAL wa_StackedVert
        #define WAL wa_Top

        wdg->appendWidget( w2 = new my_Widget( wdg, h_Rect(100,20),WAL) );
        w2->m_Bitmap = bmp;
        w2->m_Surface = srf;
        wdg->appendWidget( new h_WdgLabel( wdg, h_Rect(100,20),WAL, "label") );
        wdg->appendWidget( new h_WdgButton(wdg, h_Rect(100,20),WAL, "button") );
        wdg->appendWidget( new h_WdgValue( wdg, h_Rect(100,20),WAL, 0.3) );

        //h_Parameter* par;

        wdg->appendWidget( wdg2 = new h_WdgSlider(wdg, h_Rect(100,20),WAL) );
        m_Editor->connect( m_Parameters->item(2), wdg2 );

        wdg->appendWidget( wdg2 = new h_WdgSlider(wdg, h_Rect(100,20),WAL) );
        m_Editor->connect( m_Parameters->item(3), wdg2 );

        wdg->appendWidget( wdg2 = new h_WdgSlider(wdg, h_Rect(100,20),WAL) );
        m_Editor->connect( m_Parameters->item(4), wdg2 );

        wdg->appendWidget( wdg2 = new h_WdgSlider(wdg, h_Rect(100,20),WAL) );
        m_Editor->connect( m_Parameters->item(5), wdg2 );

        m_Editor->do_SetSize(rect.w,rect.h); // paints buffer
        m_Editor->show();
        return (void*)m_Editor;

        #undef WAL
      }

    //----------

    virtual void do_CloseEditor(void)
      {
        m_Editor->hide();
        delete bmp;
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

    virtual void do_HandleState(int a_State)
      {
        switch(a_State)
        {
          case is_Resume:
            //transferParameters();
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

#include "holos.cpp"


