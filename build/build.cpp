
// #include "build/test/test_gui2.cpp"

#include "holos.h"
#include "gui/wdg/wdg_Background.h"

//----------------------------------------------------------------------

//#include <stdio.h> // printf

//----------

class my_Descriptor : public h_Descriptor
{
  public:
    my_Descriptor() //: h_Descriptor()
      {
        m_Flags |= df_HasEditor;
      }
};

//----------

class my_Instance : public h_Instance
{
  public:
    my_Instance(h_Descriptor_Base* a_Descriptor)
    : h_Instance(a_Descriptor)
      {
      }
};

//----------

class my_Editor : public h_Editor
{
  private:
    bool is_painting;
    h_Widget* wdg;
    wdg_Background* back;

  public:
    my_Editor(h_Instance_Base* a_Instance, h_Rect a_Rect, void* a_Parent)
    : h_Editor(a_Instance,a_Rect,a_Parent)
      {
        appendWidget( new wdg_Background(this,H_DARK_RED) );
        //appendWidget( new h_Widget(this,a_Rect) );
        is_painting = false;
      }

    virtual ~my_Editor()
      {
        //if (wdg) delete wdg;
        //if (back) delete back;
      }

    //virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect)
    //  {
    //    a_Painter->setBrushColor(H_RGB(128,128,128));
    //    a_Painter->fillRect( a_Rect.x, a_Rect.y, a_Rect.x2(), a_Rect.y2() );
    //  }

    virtual void do_MouseDown(int x, int y, int b, int s)
      {
        h_Painter* painter = getPainter();
        painter->setBrushColor(H_RGB(255,128,128));
        painter->fillCircle(x-10,y-10,x+10,y+10);
        is_painting = true;
        m_Listener->on_Hint((char*)"clicky-clicky");
      }

    virtual void do_MouseUp(int x, int y, int b, int s)
      {
        h_Painter* painter = getPainter();
        painter->setBrushColor(H_RGB(255,128,128));
        painter->fillCircle(x-10,y-10,x+10,y+10);
        is_painting = false;
        m_Listener->on_Hint((char*)"...and done...");
      }

    virtual void do_MouseMove(int x, int y, int s)
      {
        if (is_painting)
        {
          h_Painter* painter = getPainter();
          painter->setBrushColor(H_BLACK);
          painter->fillCircle(x-5,y-5,x+5,y+5);
        }
      }

    virtual void on_Hint(char* a_Text)
      {
        trace(a_Text);
      }

};

//----------------------------------------------------------------------

#define H_DESCRIPTOR my_Descriptor
#define H_INSTANCE   my_Instance
#define H_EDITOR     my_Editor

#include "holos.cpp"


