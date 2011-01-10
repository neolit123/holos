#include "holos.h"
#include "gui/h_Surface.h"

int main(void)
{
  static_Debug.initialize();
  static_Core.initialize();

  h_Window* win = new h_Window( h_Rect(320,240), H_NULL );
  win->show();

  h_Painter* P  ;

  // bitmap

  h_Bitmap* bmp32 = new h_Bitmap(64,64,32);
  char* buf = bmp32->getBuffer();
  for (int y=0; y<64; y++)
  {
    for (int x=0; x<64; x++)
    {
      *buf++ = 0;     // b
      *buf++ = y*4;   // g
      *buf++ = x*4;   // r
      *buf++ = 128;   // a
    }
  }

  // surface (24-bit)

  h_Surface* srf24 = new h_Surface(64,64,24);
  P = srf24->getPainter();
  P->setBrushColor( H_RGB(192,192,192) );
  P->fillRect(0,0,63,63);
  P->setPenColor( H_RGB(224,224,224) );
  P->drawRect(0,0,63,63);
  P->setPenColor( H_RGB(255,255,255) );
  P->drawCircle(0,0,63,63);
  P->setBrushColor( H_RGB(160,160,160) );
  P->fillCircle(3,3,62,62);

  // surface (32-bit)

  h_Surface* srf32 = new h_Surface(64,64,32);
  P = srf32->getPainter();
  P->setBrushColor( H_RGB(0,128,0) );
  P->fillRect(0,0,63,63);
  P->setPenColor( H_RGB(0,0,0) );
  P->drawRect(5,5,58,58);
  P->setPenColor( H_RGB(128,128,255) );
  P->drawCircle(0,0,63,63);
  P->setBrushColor( H_RGB(192,192,255) );
  P->fillCircle(3,3,62,62);

  // window

  h_Painter* painter = win->getPainter();
  h_Rect R = win->getRect();
  painter->setBrushColor( H_RGB(128,128,128) );
  painter->fillRect(R.x,R.y,R.x2(),R.y2());

  painter->setPenColor( H_RGB(255,255,255) );
  painter->drawPoint(  10, 10);
  painter->drawPoint(  12, 10);
  painter->drawPoint(  10, 12);
  painter->drawLine(   20, 10, 40, 30);
  painter->drawRect(   50, 10, 70, 30);
  painter->drawCircle( 80, 10,100, 30);
  painter->drawArc(   110, 10,130, 30, 0.0, 0.25 );

  painter->setBrushColor( H_RGB(0,0,0) );
  painter->fillRect(   50, 50, 70, 70);
  painter->fillCircle( 80, 50,100, 70);
  painter->fillArc(   110, 50,130, 70, 0.25, 0.75 );

  painter->setTextColor( H_RGB(255,192,128) );
  painter->drawText(  140, 10, "Testing 1.2.3" );

  //---

  painter->drawBitmap(    bmp32, 140,50,  0,0,64,64);
  painter->drawSurface(   srf24, 140,120, 0,0,64,64);
  painter->blendSurface(  srf32, 210,50,  0,0,64,64);
  painter->stretchSurface(srf32, 210,120,64,64, 0,0,64,64);

  win->eventLoop();
  win->hide();

  delete bmp32;
  delete srf24;
  delete srf32;
  delete win;
  return 0;
}
