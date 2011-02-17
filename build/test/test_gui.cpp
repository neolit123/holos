
//#define H_DEBUG_LOG "holos_win.log"
//#define H_DEBUG_CON
//#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE

#define H_DEBUG_MEM
#define H_DEBUG_MEM_PRINT
#define H_DEBUG_NEW

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

#include <stdio.h>  // fopen

//----------------------------------------------------------------------

class my_Widget : public h_Widget
{
  public:

    my_Widget(h_WidgetListener* a_Listener, h_Rect a_Rect, int a_Align/*=wa_None*/)
    : h_Widget(a_Listener,a_Rect,a_Align)
      {

        // bitmap loader testing...
        // todo: move most of this into the h_BitmapLoader class

// --- test png loader ---

//        char temp[H_MAX_STRINGSIZE];
//        temp[0] = 0;
//        char* path = (char*)h_GetBasePath(temp);
//        h_Strcat(path,(char*)"../extern/mverb/background.png");
//        trace("path: " << path);
//
//        FILE* fp = fopen(path,"rb");
//        fseek(fp,0,SEEK_END);
//        int size = ftell(fp);
//        trace("size: " << size);
//        fseek(fp,0,SEEK_SET);
//        char* buffer = (char*)h_Malloc(size);
//        int num = fread(buffer,1,size,fp);
//        trace("num: " << num);
//        //if (num!=size) trace("bitmap size mismatch!");
//        fclose(fp);
//
//        h_BitmapLoader bmpload;
//        bmpload.decode(buffer,size);
//        trace("width:  " << bmpload.getWidth());
//        trace("height: " << bmpload.getHeight());
//        trace("depth:  " << bmpload.getDepth());
//
//        h_Free(buffer);

// --- ---

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

        appendParameter( new h_Parameter("param","", PF_DEFAULT, 0 ) );
        appendParameter( new h_ParFloat( "float","", PF_DEFAULT, 1.5, 0,5,0.25 ) );
        appendParameter( new h_ParInt(   "int",  "", PF_DEFAULT, 3, 1,10, H_NULL) );

      }
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

class my_Instance : public h_Instance,
                    public h_WidgetListener
{
  private:
    //h_Window*       m_Window;
    h_Editor*       m_Editor;
    h_Skin*         m_Skin;

  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        //m_Window = H_NULL;
        m_Editor = H_NULL;
        m_Skin   = H_NULL;
        transferParameters();

      }

    //----------

    //virtual ~my_Instance()
    //  {
    //  }

    //----------

    virtual void  do_HandleParameter(h_Parameter* a_Parameter)
      {
        char buf[H_MAX_STRINGSIZE];
        a_Parameter->getDisplay(buf);
        trace("param: " << a_Parameter->getName().ptr() << " = " << buf );
        //trace("  index: " << a_Parameter->getIndex());
        //trace("  connect: " << a_Parameter->getConnect());
        //trace("  value: " << a_Parameter->getValue());
      }

    //----------

    virtual void* do_OpenEditor(void* ptr)
      {
        h_Rect rect = getEditorRect();
        //trace("editor rect: " << rect.x <<","<< rect.y <<","<< rect.w <<","<< rect.h);
        //m_Window = new h_Window(this,rect,ptr);
        m_Editor = new h_Editor(this,rect,ptr);

        //h_Bitmap* bmp = m_Editor->createBitmap(64,64,24);
        //bmp->prepare();

        m_Editor->setBorders(10,10,5,5);
        m_Skin = new skin_Default();
        m_Editor->applySkin(m_Skin);
        m_Editor->appendWidget( new h_WdgBackground(this) );

        h_Widget* wdg;
        h_Widget* wdg2;

        m_Editor->appendWidget(       new h_WdgPanel(m_Editor,h_Rect(128, 0),wa_Left ));
        m_Editor->appendWidget(       new h_WdgPanel(m_Editor,h_Rect(  0,64),wa_Top ));
        m_Editor->appendWidget(       new h_WdgPanel(m_Editor,h_Rect( 64, 0),wa_Right ));
        m_Editor->appendWidget( wdg = new h_WdgPanel(m_Editor,H_NULL_RECT,   wa_Client ));
          wdg->setBorders(20,20,10,10);
          wdg->appendWidget(        new my_Widget( wdg, h_Rect(100,20),wa_StackedVert) );
          wdg->appendWidget(        new h_WdgLabel( wdg, h_Rect(100,20),wa_StackedVert, "label") );
          wdg->appendWidget(        new h_WdgButton(wdg, h_Rect(100,20),wa_StackedVert, "button") );
          wdg->appendWidget(        new h_WdgValue( wdg, h_Rect(100,20),wa_StackedVert, 0.3) );
          wdg->appendWidget( wdg2 = new h_WdgSlider(wdg, h_Rect(100,20),wa_StackedVert, 0.3) );

        h_Parameter* par = m_Parameters->item(2);
        m_Editor->connect( par, wdg2 );

        // not needed for standalone?
        // it will get a size-event and we realign there..
        // fix this!!!
        #ifdef H_LIB
          m_Editor->do_Realign();
        #endif

        //h_Painter* pnt = m_Editor->getPainter();
        //trace(pnt);
        //pnt->drawBitmap(bmp,0,0,0,0,100,100);

        m_Editor->show();
        //m_Window->setCursor(cu_Finger);
        return (void*)m_Editor;
      }

    //----------

    virtual void do_CloseEditor(void)
      {
        m_Editor->hide();
        delete m_Editor;
        m_Editor = H_NULL;
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

//#include "holos_impl.h"
#include "holos.cpp"


