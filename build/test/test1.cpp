//#define H_DEBUG_LOG "holos_win.log"
//#define H_DEBUG_CON
//#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE
//#define H_AUTOSYNC

#include "holos.h"

#include "src/h_Window.h"
#include "src/h_WdgBackground.h"
#include "src/h_SkinDefault.h"

//----------------------------------------------------------------------

char* my_str[] =
{
  (char*)"string1",
  (char*)"txt2",
  (char*)"line3"
};

// unspecified defaults to 0?
h_Parameter2 my_Params2[] =
{
  { "param1", 0.5 },
  { "param2", 0.1, h_ParPow2 },
  { "param3", 2,   H_NULL/*h_ParInt*/, 0,1,2, my_str }
};

//----------------------------------------------------------------------

class my_Widget : public h_Widget
{
  private:

  public:

    my_Widget(h_WidgetListener* a_Listener, h_Rect a_Rect, int a_Align/*=wa_None*/)
    : h_Widget(a_Listener,a_Rect,a_Align)
      {
        // test

        trace( "m_Name: " << my_Params2[0].m_Name.ptr() );
        trace( "m_Value: " << my_Params2[0].m_Value );
        trace( hex << "m_Func: 0x" << (int)my_Params2[0].m_Func );
        trace( "m_Min: " << my_Params2[0].m_Min );
        trace( "m_Max: " << my_Params2[0].m_Max );
        trace( "m_Step: " << my_Params2[0].m_Step );
        trace( "m_Strings: " << my_Params2[0].m_Strings );

        // getValue
        float val = my_Params2[1].m_Value;
        trace( "val: " << val );
        float val2 = my_Params2[1].getValue(val);
        trace( "getValue: " << val2 );

        // setValue

        // getDisplay:

        float val3 = my_Params2[2].getValue(val);
        trace( "val3: " << val3 );
        char* txt = my_Params2[2].m_Strings[ (int)val3 ];
        trace( "strings: " << txt );

      }

    virtual ~my_Widget()
      {
      }
    virtual void do_MouseDown(int x, int y, int b, int s)
      {
        m_Listener->on_Hint("clickety");
      }
    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        a_Painter->setFillColor( H_RGB(160,112,112) );
        a_Painter->fillRect(m_Rect.x, m_Rect.y, m_Rect.x2(), m_Rect.y2());
        a_Painter->setTextColor( H_RGB(255,255,255) );
        a_Painter->drawText(m_Rect.x+10,m_Rect.y+10,(char*)"hello world!");
      }

};

//----------------------------------------------------------------------

//h_ParamDescr my_Params[] =
//{
//  {"param1","",pt_None,    PF_DEFAULT, 0,   /*0,1,0*/},
//  {"param2","",pt_None,    PF_DEFAULT, 0.5,   /*0,1,0*/},
//  {"param3","",pt_None,    PF_DEFAULT, 1,   /*0,1,0*/}
////  {"param2","",pt_Float,   PF_DEFAULT, 0.5, 0,1,0 },
////  {"param3","",pt_FloatPow,PF_DEFAULT, 1.0, 0,1,0, 2}
//};
//
////----------------------------------------------------------------------
//
//h_Descriptor my_Descriptor =
//{
//  "build.cpp",
//  "ccernn",
//  "holos test plugin",
//  0,
//  H_MAGIC,
//  df_HasEditor | df_ReceiveMidi,//df_None,
//  2,    // inputs
//  2,    // outputs
//  3,    // parameters
//  0,    // programs
//  my_Params,
//  H_NULL,
//  h_Rect(512,256)
//};
//
//#define H_DESCRIPTOR my_Descriptor

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
        m_EditorRect  = h_Rect(0,0,255,255);
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
            transferParameters();
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

//TODO: move most of this into the h_Editor

    virtual void* do_OpenEditor(void* ptr)
      {
        h_Rect rect = getEditorRect();
        m_Window = new h_Window(this,rect,ptr);
        m_Skin = new skin_Default();
        m_Window->applySkin(m_Skin);
        m_Window->appendWidget( new h_WdgBackground(this) );
        m_Window->appendWidget( new my_Widget(this,h_Rect(10,10,100,100),wa_Client) );
        m_Window->do_Realign();
        m_Window->show();
        m_Window->setCursor(cu_Finger);
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

    //----------

    virtual void on_Hint(h_String a_Text)
      {
        trace("my_Instance.on_Hint: " << a_Text.ptr());
      }


};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"


