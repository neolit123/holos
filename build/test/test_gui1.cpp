#include "holos.h"
#include "lib/h_Rand.h"
#include "gui/h_Surface.h"
#include "gui/h_Widget.h"

#include "gui/wdg/wdg_Background.h"
//#include "gui/wdg/wdg_Color.h"
//#include "gui/wdg/wdg_Panel.h"

//----------------------------------------------------------------------

class my_Window : public h_Window,
                  public h_WidgetListener
{
  public:
    my_Window(h_Rect a_Rect, void* a_Parent)
    : h_Window(a_Rect,a_Parent)
      {
      }
    virtual ~my_Window()
      {
      }

    // widget listener
    virtual void on_Change(h_Widget* a_Widget) { /*trace("on_Change");*/ }
    //virtual void on_SetHint(char* a_Text) {}

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

int main(void)
{
  static_Debug.initialize();
  static_Core.initialize();

  my_Window* window = new my_Window( h_Rect(640,480), H_NULL );
  wdg_Background* background = new wdg_Background(window,H_GREY);
  window->setRoot(background);
  window->show();
  window->eventLoop();
  window->hide();

//  delete bmp32;
//  delete srf24;
//  delete srf32;
  delete background;
  delete window;
  return 0;
}

//----------------------------------------------------------------------

//  h_Painter* P;
//
//  //----------------------------------------
//  // prepare
//  //----------------------------------------
//
//  // bitmap (32-bit)
//
//  //unsigned long buffer[64*64];
//  h_Bitmap* bmp32 = new h_Bitmap(64,64,32,H_NULL/*,buffer*/);
//  char* buf = (char*)bmp32->getBuffer();
//  //char* buf = (char*)&buffer;
//
//  for (int y=0; y<64; y++)
//  {
//    for (int x=0; x<64; x++)
//    {
//      *buf++ = 0;     // b
//      *buf++ = y*4;   // g
//      *buf++ = x*4;   // r
//      *buf++ = 128;   // a
//    }
//  }
//  bmp32->prepare();
//
//  // surface (24-bit)
//
//  h_Surface* srf24 = new h_Surface(64,64,24);
//  P = srf24->getPainter();
//  P->setBrushColor( H_RGB(192,192,192) );
//  P->fillRect(0,0,63,63);
//  P->setPenColor( H_RGB(224,224,224) );
//  P->drawRect(0,0,63,63);
//  P->setPenColor( H_RGB(255,255,255) );
//  P->drawCircle(0,0,63,63);
//  P->setBrushColor( H_RGB(160,160,160) );
//  P->fillCircle(3,3,62,62);
//
//  // surface (32-bit)
//
//  h_Surface* srf32 = new h_Surface(64,64,32);
//  P = srf32->getPainter();
//  P->setBrushColor( H_RGB(0,128,0) );
//  P->fillRect(0,0,63,63);
//  P->setPenColor( H_RGB(0,0,0) );
//  P->drawRect(5,5,58,58);
//  P->setPenColor( H_RGB(128,128,255) );
//  P->drawCircle(0,0,63,63);
//  P->setBrushColor( H_RGB(64,64,128) );
//  P->fillCircle(3,3,62,62);
//
//  //----------------------------------------
//  // draw
//  //----------------------------------------
//
//  h_Painter* painter = win->getPainter();
//
//  // backgorund
//  h_Rect R = win->getRect();
//  painter->setBrushColor(H_DARK_GREY);
//  painter->fillRect(R.x,R.y,R.x2(),R.y2());
//  // draw
//  painter->setPenColor(H_RED);
//  painter->drawPoint(  10, 10);
//  painter->drawPoint(  12, 10);
//  painter->drawPoint(  10, 12);
//  painter->drawLine(   20, 10, 40, 30);
//  painter->drawRect(   50, 10, 70, 30);
//  painter->drawCircle( 80, 10,100, 30);
//  painter->drawArc(   110, 10,130, 30, 0.0, 0.25 );
//  // fill
//  painter->setBrushColor(H_GREY);
//  painter->fillRect(   50, 50, 70, 70);
//  painter->fillCircle( 80, 50,100, 70);
//  painter->fillArc(   110, 50,130, 70, 0.25, 0.75 );
//  // text
//  painter->setTextColor(H_GREEN);
//  painter->drawText(  140, 10, "Testing 1.2.3" );
//  // blit
//  painter->drawBitmap(    bmp32, 140,50,  0,0,64,64);
//  painter->drawSurface(   srf24, 140,120, 0,0,64,64);
//  painter->blendSurface(  srf32, 210,50,  0,0,64,64);
//  painter->stretchSurface(srf32, 210,120,128,128, 0,0,64,64);
//
//  //----------------------------------------
//  // benchmark
//  //----------------------------------------
//
//  // totally unscientific, counted ticks on a wall clock...
//  // win7
//  //   drawBitmap,   14 sec (ca 71-500 per sec)
//  //   drawSurface,  10 sec (-"- 100.000 -"-)
//  //   blendSurface, 11 sec (91.000
//  // linux
//  //   drawBitmap,   ? sec
//  //   drawSurface,  ? sec
//  //   blendSurface, ? sec
//
//  //  for (int i=0; i<1000000; i++)
//  //  {
//  //    int x = h_RandInt(280);
//  //    int y = h_RandInt(200);
//  //    int w = 40;
//  //    int h = 40;
//  //    painter->drawBitmap(  bmp32, x,y, 0,0,w,h);
//  ////    painter->drawSurface( srf32, x,y, 0,0,w,h);
//  ////    painter->blendSurface(srf32, x,y, 0,0,w,h);
//  ////    painter->drawSurface( srf24, x,y, 0,0,w,h);
//  ////    painter->blendSurface(srf24, x,y, 0,0,w,h);
//  //  }
