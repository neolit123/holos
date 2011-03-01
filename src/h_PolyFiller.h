/*
  Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al

  This file is part of the Holos Library.
  http://holos.googlecode.com

  the Holos Library is free software: you can redistribute it and/or modify
  it under the terms of the Holos Library License, either version 1.0
  of the License, or (at your option) any later version.

  the Holos Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See LICENSE_HOLOS for more details.

  You should have received a copy of the Holos Library License
  If not, see <http://holos.googlecode.com/>.
*/
//----------------------------------------------------------------------
#ifndef h_PolyFiller_included
#define h_PolyFiller_included
//----------------------------------------------------------------------

//#include ""

class h_PolyFiller //: public h_
{
  public:
    h_PolyFiller() /*: h_()*/ {}
    ~h_PolyFiller() {}
};

//----------------------------------------------------------------------
#endif













































/*

//
//
//
//
//
//
//#ifndef axBitmapRender_included
//#define axBitmapRender_included
////----------------------------------------------------------------------
//// assumes 32-bit
//// LOTS of optimization could be done here!"
//// also, check format rgba vs bgra, etc...
//
//#include "gui/axBitmap.h"
//
//#define USE_AFFINE
////#define USE_EXACT_PERSPECTIVE
////#define USE_SUB_AFFINE_PERSPECTIVE
//
//#define SUB_SHIFT 4 // Sub-affine span size
//#define SUB_SPAN (1<<SUB_SHIFT)
//#define	MIN(a,b) ((a)<(b)?(a):(b))
//
//struct axPolyVertex
//{
//  float	u, v, w;
//  float	x, y, z;
//  int	iy;
//  axPolyVertex* next;
//};
//
//struct axPolyEdge
//{
//  float	u, du;
//  float	v, dv;
//  float	w, dw;
//  float	x, dx;
//  int	height;
//};
//
//
//class axBitmapRender
//{
//  private:
//    axBitmap*     mBitmap;
//    int           mWidth;
//    int           mHeight;
//    unsigned int* mBuffer;
//
//  public:
//
//    axBitmapRender()
//      {
//      }
//
//    virtual ~axBitmapRender()
//      {
//      }
//
//    //--------------------------------------------------
//
//    void setup(axBitmap* aBitmap)
//      {
//        mBitmap = aBitmap;
//        mWidth  = aBitmap->getWidth();
//        mHeight = aBitmap->getHeight();
//        mBuffer = (unsigned int*)aBitmap->getBuffer();
//      }
//
//    //--------------------------------------------------
//
//    //void clear(void) { axMemset(mBuffer,0,mWidth*mHeight*4); }
//    //unsigned int getPixel(int x, int y) { return mBuffer[y*mWidth+x]; }
//    //void setPixel(int x, int y, unsigned int col) { mBuffer[y*mWidth+x] = col; }
//
//
//    //----------
//
//    // fixed point (.8) multiplication
//    inline unsigned char mul_fp8(int c, int a)
//      {
//        int ret = (c*a) >> 8;
//        return ret & 0xff;
//      }
//
//    //----------
//
//    void blendPixel(unsigned int* ptr, unsigned int color, unsigned char alpha)
//      {
//        // split color into separate r,g,b,a
//        unsigned int b1 = (color&0x000000ff);
//        unsigned int g1 = (color&0x0000ff00) >> 8;
//        unsigned int r1 = (color&0x00ff0000) >> 16;
//        unsigned int a1 = (color&0xff000000) >> 24;
//        // pre mix-in with master-alpha
//        alpha = mul_fp8(alpha,a1);
//        //alpha = mul_fp8(alpha,a1);
//        // read pixel from buffer
//        unsigned int c  = *ptr;//getPixel(x,y);
//        // extract r,g,b
//        unsigned int b2 = (c&0x0000ff);
//        unsigned int g2 = (c&0x00ff00) >> 8;
//        unsigned int r2 = (c&0xff0000) >> 16;
//        // alpha-blend the two colors
//        unsigned char b  = mul_fp8(b1,alpha) + mul_fp8(b2,255-alpha);
//        unsigned char g  = mul_fp8(g1,alpha) + mul_fp8(g2,255-alpha);
//        unsigned char r  = mul_fp8(r1,alpha) + mul_fp8(r2,255-alpha);
//        // and write it to buffer
//        //setPixel(x,y,(r<<16)+(g<<8)+b);
//        *ptr = (r<<16)+(g<<8)+b;
//      }
//
//
//    void blendPixel(int x, int y, unsigned int color, unsigned char alpha)
//      {
//        unsigned int* ptr = &mBuffer[y*mWidth+x];
//        blendPixel(ptr,color,alpha);
//      }
//
//    //--------------------------------------------------
//    // antialiased, transparent line
//    //--------------------------------------------------
//
//    // http://www.users.on.net/~swcheetah/sam/Prog.html
//
//    #define _frac(x)    (x-(int)x)
//    #define _invfrac(x) (1-_frac(x))
//
//    //----------
//
//    void drawLineAA(float x1, float y1, float x2, float y2, unsigned int color)
//      {
//        float grad, xd, yd, xm, ym, xend, yend, xf, yf, brightness1, brightness2;
//        int x, y, ix1, ix2, iy1, iy2;
//        unsigned char c1, c2;
//        // width and height of the line
//        xd = (x2-x1);
//        yd = (y2-y1);
//        if (axAbs(xd)>axAbs(yd))
//        {
//          // horizontal-ish lines
//          if(x1>x2)
//          {
//            xm=x1; x1=x2; x2=xm;                      // Swap x1 with x2
//            ym=y1; y1=y2; y2=ym;                      // Swap y1 with y2
//            xd = (x2-x1);                             // Calculate line width
//            yd = (y2-y1);                             // Calculate line height
//          }
//          grad = yd / xd;                             // Calculate gradient
//          // End Point 1
//          xend = (int)(x1+.5);                        // Find nearest integer X-coordinate
//          yend = y1 + grad*(xend-x1);                 // and corresponding Y value
//          ix1  = (int)(xend);                         // calc screen coordinates
//          iy1  = (int)(yend);
//          yf   = yend+grad;                           // calc first Y-intersection for main loop
//          // end point 2
//          xend = (int)(x2+.5);                        // find nearest integer X-coordinate
//          yend = y2 + grad*(xend-x2);                 // and corresponding Y value
//          ix2  = (int)(xend);                         // calc screen coordinates
//          iy2  = (int)(yend);
//          // main loop
//          //for (x=ix1+1; x<=ix2-1; x++)
//          for (x=ix1+1; x<ix2; x++)
//          {
//            brightness1 = _invfrac(yf);                // calc pixel brightnesses
//            brightness2 =    _frac(yf);
//            c1 = (unsigned char)(brightness1 * 255);  // calc pixel values
//            c2 = (unsigned char)(brightness2 * 255);
//            blendPixel(x,(int)yf,  color,c1);         // draw the pair of pixels
//            blendPixel(x,(int)yf+1,color,c2);
//            yf += grad;
//          }
//        }
//        else
//        {
//          // Vertical(ish) lines
//          if(y1>y2)
//          {
//            xm = x1; x1 = x2; x2 = xm; //Swap x1 with x2
//            ym = y1; y1 = y2; y2 = ym; //Swap y1 with y2
//            xd = (x2 - x1);            //Calculate line width
//            yd = (y2 - y1);            //Calculate line height
//          }
//          grad = xd / yd;                    //Calculate gradient
//          //End Point 1
//          xend = (int)(x1+.5);              //Find nearest integer X-coordinate
//          yend = y1 + grad*(xend-x1);       //and corresponding Y value
//          ix1 = (int)(xend);                 //calc screen coordinates
//          iy1 = (int)(yend);
//          xf = xend+grad;                      //calc first Y-intersection for main loop
//          //End Point 2
//          xend = (int)(x2+.5);                 //find nearest integer X-coordinate
//          yend = y2 + grad*(xend-x2);          //and corresponding Y value
//          ix2 = (int)(xend);                   //calc screen coordinates
//          iy2 = (int)(yend);
//          //MAIN LOOP
//          //for(y=iy1+1;y<=iy2-1;y++)
//          for(y=iy1+1;y<iy2;y++)
//          {
//            brightness1 = _invfrac(xf);    //calc pixel brightnesses
//            brightness2 =    _frac(xf);
//            c1 = (unsigned char)(brightness1 * 255);//calc pixel values
//            c2 = (unsigned char)(brightness2 * 255);
//            blendPixel((int)xf,  y,color,c1);      //draw the pair of pixels
//            blendPixel((int)xf+1,y,color,c2);
//            xf += grad;
//          }
//        }
//      }
//
//    #undef _frac
//    #undef _invfrac
//
//    //--------------------------------------------------
//    // texture mapping
//    //--------------------------------------------------
//
//    inline void calcEdgeDeltas(axPolyEdge &edge, axPolyVertex *top, axPolyVertex *bot)
//      {
//        // Edge deltas
//        float	overHeight = 1.0 / (bot->y - top->y);
//        edge.du = (bot->u - top->u) * overHeight;
//        edge.dv = (bot->v - top->v) * overHeight;
//        #ifndef USE_AFFINE
//        edge.dw = (bot->w - top->w) * overHeight;
//        #endif
//        edge.dx = (bot->x - top->x) * overHeight;
//        // Screen pixel Adjustments (some call this "sub-pixel accuracy")
//        float	subPix = (float) top->iy - top->y;
//        edge.u  = top->u + edge.du * subPix;
//        edge.v  = top->v + edge.dv * subPix;
//        #ifndef USE_AFFINE
//        edge.w  = top->w + edge.dw * subPix;
//        #endif
//        edge.x  = top->x + edge.dx * subPix;
//      }
//
//    //----------
//
//    //#define p_Wireframe     1
//    //#define p_Color         2
//    //#define p_Gouraud       3
//    //#define p_TexAffine     4
//    //#define p_TexPersp      5
//    //#define p_TexFullPersp  6
//
//    void drawPolyTex(axPolyVertex *verts, axBitmap* aTexture, bool aAlpha)
//      {
//        int RES_X = mWidth;
//        //int RES_Y = mHeight;
//        int TEX_X = aTexture->getWidth();
//        int TEX_Y = aTexture->getHeight();
//        unsigned int* buffer = mBuffer;
//        unsigned int* texture = (unsigned int*)aTexture->getBuffer();
//
//        // Find the top-most vertex
//        axPolyVertex *v = verts;
//        axPolyVertex *lastVert = verts;
//        axPolyVertex *lTop = verts;
//        axPolyVertex *rTop;
//        while(v)
//        {
//          if (v->y < lTop->y) lTop = v;
//          lastVert = v;
//          v->iy = axCeil(v->y);
//          v = v->next;
//        }
//        rTop = lTop;
//        // Top scanline of the polygon in the frame buffer
//        unsigned int* fb = &buffer[lTop->iy * RES_X];
//        // Left & Right edges (primed with 0 to force edge calcs first-time through)
//        axPolyEdge	le, re;
//        le.height = 0;
//        re.height = 0;
//        // Render the polygon
//        while(1)
//        {
//          if (!le.height)
//          {
//            axPolyVertex	*lBot = lTop - 1; if (lBot < verts) lBot = lastVert;
//            le.height = lBot->iy - lTop->iy;
//            if (le.height < 0) return;
//            calcEdgeDeltas(le, lTop, lBot);
//            lTop = lBot;
//          }
//          if (!re.height)
//          {
//            axPolyVertex	*rBot = rTop + 1; if (rBot > lastVert) rBot = verts;
//            re.height = rBot->iy - rTop->iy;
//            if (re.height < 0) return;
//            calcEdgeDeltas(re, rTop, rBot);
//            rTop = rBot;
//          }
//          // Polygon must have height
//          if (!le.height && !re.height) return;
//          // Get the height
//          int	height = MIN(le.height, re.height);
//          // Subtract the height from each edge
//          le.height -= height;
//          re.height -= height;
//          // Render the current trapezoid defined by left & right edges
//          while(height-- > 0)
//          {
//            // Texture coordinates
//            float	overWidth = 1.0 / (re.x - le.x);
//            float	du  = (re.u - le.u) * overWidth;
//            float	dv  = (re.v - le.v) * overWidth;
//            int	idu = int(du * 65536.0);
//            int	idv = int(dv * 65536.0);
//            // Find the end-points
//            int	start = (int) axCeil(le.x);
//            int	end   = (int) axCeil(re.x);
//            // Texture adjustment (some call this "sub-texel accuracy")
//            float	subTex = (float) start - le.x;
//            int	iu = int((le.u + du * subTex) * 65536.0);
//            int	iv = int((le.v + dv * subTex) * 65536.0);
//            // Fill the entire span
//            //char	*span = fb + start;
//            unsigned int* span = fb + start;
//            if (aAlpha)
//            {
//              for (; start < end; start++)
//              {
//                int _u = (iu>>16) % TEX_X;
//                int _v = (iv>>16) % TEX_Y;
//                unsigned int pixel = texture[_u + _v*TEX_X];
//                blendPixel(span++,pixel,255);
//                iu += idu;
//                iv += idv;
//              }
//            }
//            else
//            {
//              for (; start < end; start++)
//              {
//                // *(span++) += texture[((iv>>10)&0xffffffC0) + (iu>>16)];       // add (blend)
//                // *(span++)  = texture[((iv>>10)&0xffffffC0) + (iu>>16)];       // set (overwrite)
//                int _u = (iu>>16) % TEX_X;
//                int _v = (iv>>16) % TEX_Y;
//                unsigned int pixel = texture[_u + _v*TEX_X];
//                *(span++) = pixel;
//                iu += idu;
//                iv += idv;
//              }
//            }
//            // Step
//            le.u += le.du;
//            le.v += le.dv;
//            le.x += le.dx;
//            re.u += re.du;
//            re.v += re.dv;
//            re.x += re.dx;
//            fb   += RES_X;
//          }
//        }
//      }
//
//};
//
////----------------------------------------------------------------------
//#endif
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//#if 0
//
//
//// ----------------------------------------------------------------------------
//// Tmap.cpp - Texture mapper example source, focusing on precision
////
//// Copyright 1999, Paul D. Nettle.  All rights reserved.
////
//// ----------------------------------------------------------------------------
////
//// NOTES ABOUT THE ROUTINES BELOW:
////
//// This file can be compiled in Watcom, using the following command-line:
////
////    wcl386 main.cpp /l#dos4g /en /fpi /fp5 /5r /w4 /zq /zp1 /oneatx /s
////
////
//// When drawing, there are four polygons plotted (in perspective) that combine
//// to create a single continuous surface.  This is done to test for overlaps
//// in adjacent polygons.
////
//// Each polygon routine acheives results as accurate as the algorithm will
//// allow.  See the comments above each routine for details of accuracy issues.
////
//// Each polygon routine performs no wrapping (and the wrapping error should be
//// visible if there is overflow error.)  They also ADD each pixel to the screen,
//// rather than simply plotting them, to show any overlapping of adjacent
//// polygons.
////
//// Each routine is hard-coded for a 64x64 texture.  If you change the texture
//// size defines (i.e. TEX_X & TEX_Y) you need to change the inner-loops of the
//// texture mappers.
////
//// Vertices must be in clock-wise order.
////
//// The routines SHOULD not crash with concave polygons, but may only render
//// a portion of the given polygon (and incorrectly at that.)
////
//// Any other notes about a specific algorithm are given above each polgyon
//// routine.
////
//// ----------------------------------------------------------------------------
////
//// OTHER NOTES:
////
//// The routines may not properly handle polygons that have three vertices in
//// a straight line across the top of the polygon (causing it to bail too early)
////
//// The routines SHOULD not crash with concave polygons, but may only render
//// a portion of the given polygon (and incorrectly at that.)
////
//// ----------------------------------------------------------------------------
//
//#include <stdio.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <mem.h>
//#include <math.h>
//
//// ----------------------------------------------------------------------------
//// THESE FLAGS CONTROL WHAT KIND OF TEXTURE MAPPER TO USE (PICK ONLY ONE!)
//// ----------------------------------------------------------------------------
//
////#define USE_AFFINE
////#define USE_EXACT_PERSPECTIVE
//#define USE_SUB_AFFINE_PERSPECTIVE
//
//// ----------------------------------------------------------------------------
//// The vertex structure.  Note that this uses a linked list.  I tend to prefer
//// them for ease of managing polygons with large numbers of dynamic vertices,
//// though lists will work fine, too.
//// ----------------------------------------------------------------------------
//
//typedef	struct	vertex
//{
//	float	u, v, w;
//	float	x, y, z;
//	int	iy;
//	struct	vertex *next;
//} sVERT;
//
//// ----------------------------------------------------------------------------
//// The edge structure.  This is used to keep track of each left & right edge
//// during scan conversion.  The algorithm does not pre-build an edge list
//// prior to rendering, rather it renders edges as it builds them.  This
//// structure is used only to keep the variables together.
//// ----------------------------------------------------------------------------
//
//typedef	struct	edge
//{
//	float	u, du;
//	float	v, dv;
//	float	w, dw;
//	float	x, dx;
//	int	height;
//} sEDGE;
//
//// ----------------------------------------------------------------------------
//// Simple routine to get the Pentium(tm) high-resolution timer.
//// ----------------------------------------------------------------------------
//
//void	getTicks(const unsigned int *hi, const unsigned int *lo);
//
//#pragma aux getTicks =		\
//	"rdtsc"			\
//	"mov	[esi],edx"	\
//	"mov	[edi],eax"	\
//	parm caller [esi] [edi]	\
//	modify nomemory exact [eax edx esi edi];
//
//// ----------------------------------------------------------------------------
//// Set the given VGA mode (the polygon routines expect 0x13)
//// ----------------------------------------------------------------------------
//
//void	setMode(const unsigned int mode);
//
//#pragma aux setMode =		\
//	"int	0x10"		\
//	parm caller [eax]	\
//	modify nomemory exact [eax];
//
//// ----------------------------------------------------------------------------
//// Constants
//// ----------------------------------------------------------------------------
//
//enum	{RES_X = 320};			// Screen resolution
//enum	{RES_Y = 200};			//
//
//enum	{TEX_X = 64};			// Texture resolution
//enum	{TEX_Y = 64};			//
//
//enum	{POLY_COUNT = 10000};		// Number of polygons to draw
//
//enum	{SUB_SHIFT = 4};		// Sub-affine span size
//enum	{SUB_SPAN = 1 << SUB_SHIFT};	//
//
//// ----------------------------------------------------------------------------
//// This is handy
//// ----------------------------------------------------------------------------
//
//#define	MIN(a, b)	((a) < (b) ? (a) : (b))
//
//// ----------------------------------------------------------------------------
//// Buffers.  This is where we store the texture and the buffer we render into.
//// ----------------------------------------------------------------------------
//
//static	char	textureBuffer[TEX_X * TEX_Y];
//static	char	backBuffer[RES_X * RES_Y];
//static	char	*frameBuffer = (char *) 0xA0000;
//
//// ----------------------------------------------------------------------------
//// Draws a checkerboard texture into textureBuffer.
//// ----------------------------------------------------------------------------
//
//void	drawTexture()
//{
//	// Frequency: the lower the number, the higher the frequency
//
//	const int	freq = 2;
//	const int	fAnd = 1 << freq;
//
//	for (int y = 0; y < TEX_Y; y++)
//	{
//		int	yIndex = y * TEX_X;
//
//		for (int x = 0; x < TEX_X; x++)
//		{
//			textureBuffer[yIndex+x] = (y&fAnd) == (x&fAnd) ? 1:15;
//		}
//	}
//}
//
//// ----------------------------------------------------------------------------
//// Calculate the deltas along an edge.  This routine is called once per edge
//// per polygon.  Notice how the affine does not require the calculation of the
//// homogenous coordinate (w).
//// ----------------------------------------------------------------------------
//
//inline	calcEdgeDeltas(sEDGE &edge, sVERT *top, sVERT *bot)
//{
//	// Edge deltas
//
//	float	overHeight = 1.0 / (bot->y - top->y);
//	edge.du = (bot->u - top->u) * overHeight;
//	edge.dv = (bot->v - top->v) * overHeight;
//	#ifndef USE_AFFINE
//	edge.dw = (bot->w - top->w) * overHeight;
//	#endif
//	edge.dx = (bot->x - top->x) * overHeight;
//
//	// Screen pixel Adjustments (some call this "sub-pixel accuracy")
//
//	float	subPix = (float) top->iy - top->y;
//	edge.u  = top->u + edge.du * subPix;
//	edge.v  = top->v + edge.dv * subPix;
//	#ifndef USE_AFFINE
//	edge.w  = top->w + edge.dw * subPix;
//	#endif
//	edge.x  = top->x + edge.dx * subPix;
//}
//
//// ----------------------------------------------------------------------------
//// Draw an affine texture-mapped polygon.
////
//// With a simple affine texture mapper (and without the use of sub-texel
//// accuracy) the final pixel on each scanline of the polygon references the
//// texel along that edge.  If the polygon uses the entire texture, then that
//// last pixel will be out of bounds of the texture.  For example, a 4-sided
//// polygon might reference these UV values:
////
////    [0,0] [1,0]                             [0,0] [64,0]
////      +-----+                                 +-----+
////      |     |                                 |     |
////      |     |    given a 64x64 texture:       |     |
////      |     |                                 |     |
////      |     |                                 |     |
////      +-----+                                 +-----+
////    [0,1] [1,1]                            [0,64] [64,64]
////
//// Note that the edges on the right reference the texel that is just beyond the
//// range of the texture map (0-63 does not include 64).  This is safe, since
//// these polygons are rendered top/left, which means that the far right edge
//// and the last scanline of the polygon is not drawn (to avoid overlapping of
//// adjacent polygons.)
////
//// Normal affine texture mapping has only one form of accuracy loss which can
//// not be avoided.  The texture mapper interpolates (i.e. adds a delta to each
//// U/V value per pixel) which accumulates error, since the deltas calculated
//// are only stored at the resolution that the floating point unit will provide.
//// Very few numbers can be represented exactly in IEEE floating point, so the
//// closest representative is stored instead.  This inaccuracy is accumulated
//// as values are interpolated from pixel to pixel, accumulating the error in
//// the deltas.  Add this to the error of the original U/V value where the
//// interpolation began (also due to the inability to store an exact value) and
//// the error is still small, but not negligable.
////
//// This can be reduced by using higher precision floating point values (i.e.
//// using doubles rather than floats.)  But this sill only reduces the problem,
//// and does not solve the problem entirely.
////
//// This problem can manifest itself in a few ways.  First, it can cause
//// inaccurate texel selections, and cause slight jitters in the texture.  It
//// can cause overflows in the texture (i.e. the right edge of the span may
//// reference a texel beyond the range of the texture).  It may also cause
//// slight inaccuracies along each edge of the polygon, choosing to render to
//// the wrong pixel.
////
//// These problems are very rare, indeed, and may never be visually noticed.
//// Especially the last error (choosing the wrong pixel along the edges of the
//// polygon) since the adjacent polygons will most likely make the same wrong
//// choice, if the two adjacent polygons share their edge vertices.
////
//// There is no cure for this inaccuracy, given the algorithms used.  However,
//// there is an acceptable work-around.  Simply choosing UV values that are just
//// INSIDE the bounds of the texture can solve this problem.  Also, allowing
//// wrapping textures can also solve this problem, provided the overflow wraps
//// to a texel that "looks right."
//// ----------------------------------------------------------------------------
//
//void	drawAffineTexturedPolygon(sVERT *verts)
//{
//	// Find the top-most vertex
//
//	sVERT	*v = verts, *lastVert = verts, *lTop = verts, *rTop;
//
//	while(v)
//	{
//		if (v->y < lTop->y) lTop = v;
//		lastVert = v;
//		v->iy = ceil(v->y);
//		v = v->next;
//	}
//
//	rTop = lTop;
//
//	// Top scanline of the polygon in the frame buffer
//
//	char	*fb = &backBuffer[lTop->iy * RES_X];
//
//	// Left & Right edges (primed with 0 to force edge calcs first-time through)
//
//	sEDGE	le, re;
//	le.height = 0;
//	re.height = 0;
//
//	// Render the polygon
//
//	while(1)
//	{
//		if (!le.height)
//		{
//			sVERT	*lBot = lTop - 1; if (lBot < verts) lBot = lastVert;
//			le.height = lBot->iy - lTop->iy;
//			if (le.height < 0) return;
//			calcEdgeDeltas(le, lTop, lBot);
//			lTop = lBot;
//		}
//
//		if (!re.height)
//		{
//			sVERT	*rBot = rTop + 1; if (rBot > lastVert) rBot = verts;
//			re.height = rBot->iy - rTop->iy;
//			if (re.height < 0) return;
//			calcEdgeDeltas(re, rTop, rBot);
//			rTop = rBot;
//		}
//
//		// Polygon must have height
//
//		if (!le.height && !re.height) return;
//
//		// Get the height
//
//		int	height = MIN(le.height, re.height);
//
//		// Subtract the height from each edge
//
//		le.height -= height;
//		re.height -= height;
//
//		// Render the current trapezoid defined by left & right edges
//
//		while(height-- > 0)
//		{
//			// Texture coordinates
//
//			float	overWidth = 1.0 / (re.x - le.x);
//			float	du  = (re.u - le.u) * overWidth;
//			float	dv  = (re.v - le.v) * overWidth;
//			int	idu = int(du * 65536.0);
//			int	idv = int(dv * 65536.0);
//
//			// Find the end-points
//
//			int	start = (int) ceil(le.x);
//			int	end   = (int) ceil(re.x);
//
//			// Texture adjustment (some call this "sub-texel accuracy")
//
//			float	subTex = (float) start - le.x;
//			int	iu = int((le.u + du * subTex) * 65536.0);
//			int	iv = int((le.v + dv * subTex) * 65536.0);
//
//			// Fill the entire span
//
//			char	*span = fb + start;
//
//			for (; start < end; start++)
//			{
//				*(span++) += textureBuffer[((iv>>10)&0xffffffC0) + (iu>>16)];
//				iu += idu;
//				iv += idv;
//			}
//
//			// Step
//
//			le.u += le.du;
//			le.v += le.dv;
//			le.x += le.dx;
//			re.u += re.du;
//			re.v += re.dv;
//			re.x += re.dx;
//			fb += RES_X;
//		}
//	}
//}
//
//// ----------------------------------------------------------------------------
//// Draw a perspective-correct texture-mapped polygon.  The following routine
//// performs perspective correction on ALL pixels.  This produces a much slower
//// routine, but at the same time, much more accurate results.
////
//// Given the inaccuracies I've already explained for the affine texture mapper,
//// this routine suffers from one more accumulation of error.  The fact that
//// the values interpolated are not their original values, rather they are
//// divided by Z.
////
//// Interpolating these u/z and v/z values accumulates the error in an amplified
//// form, so that when the values are then divided by W for each pixel, the
//// amplified error from accumulation is added to the accuracy lost from the
//// two divisions (first division by Z, then the division by W).
////
//// This error can manifest itself in the same ways that the affine version can,
//// with the exception that the error produced by the following routine is
//// amplified.
//// ----------------------------------------------------------------------------
//
//void	drawPerspectiveTexturedPolygon(sVERT *verts)
//{
//	// Find the top-most vertex
//
//	sVERT	*v = verts, *lastVert = verts, *lTop = verts, *rTop;
//
//	while(v)
//	{
//		if (v->y < lTop->y) lTop = v;
//		lastVert = v;
//		v->iy = ceil(v->y);
//		v = v->next;
//	}
//
//	rTop = lTop;
//
//	// Top scanline of the polygon in the frame buffer
//
//	char	*fb = &backBuffer[lTop->iy * RES_X];
//
//	// Left & Right edges (primed with 0)
//
//	sEDGE	le, re; le.height = 0; re.height = 0;
//
//	// Render the polygon
//
//	while(1)
//	{
//		if (!le.height)
//		{
//			sVERT	*lBot = lTop - 1; if (lBot < verts) lBot = lastVert;
//			le.height = lBot->iy - lTop->iy;
//			if (le.height < 0) return;
//			calcEdgeDeltas(le, lTop, lBot);
//			lTop = lBot;
//		}
//
//		if (!re.height)
//		{
//			sVERT	*rBot = rTop + 1; if (rBot > lastVert) rBot = verts;
//			re.height = rBot->iy - rTop->iy;
//			if (re.height < 0) return;
//			calcEdgeDeltas(re, rTop, rBot);
//			rTop = rBot;
//		}
//
//		// Polygon must have height
//
//		if (!le.height && !re.height) return;
//
//		// Get the height
//
//		int	height = MIN(le.height, re.height);
//
//		// Subtract the height from each edge
//
//		le.height -= height;
//		re.height -= height;
//
//		// Render the current trapezoid defined by left & right edges
//
//		while(height-- > 0)
//		{
//			// Texture coordinates
//
//			float	overWidth = 1.0 / (re.x - le.x);
//			float	du = (re.u - le.u) * overWidth;
//			float	dv = (re.v - le.v) * overWidth;
//			float	dw = (re.w - le.w) * overWidth;
//
//			// Find the end-points
//
//			int	start = (int) ceil(le.x);
//			int	end   = (int) ceil(re.x);
//
//			// Texture adjustment (some call this "sub-texel accuracy")
//
//			float	subTex = (float) start - le.x;
//			float	u = le.u + du * subTex;
//			float	v = le.v + dv * subTex;
//			float	w = le.w + dw * subTex;
//
//			// Fill the entire span
//
//			char	*span = fb + start;
//
//			for (; start < end; start++)
//			{
//				float	z = 1.0 / w;
//				int	s = (int) (u * z);
//				int	t = (int) (v * z);
//
//				*(span++) += textureBuffer[(t<<6)+s];
//
//				u += du;
//				v += dv;
//				w += dw;
//			}
//
//			// Step
//
//			le.u += le.du;
//			le.v += le.dv;
//			le.w += le.dw;
//			le.x += le.dx;
//			re.u += re.du;
//			re.v += re.dv;
//			re.w += re.dw;
//			re.x += re.dx;
//			fb += RES_X;
//		}
//	}
//}
//
//// ----------------------------------------------------------------------------
//// Draw a "sub-affine" perspective-correct texture-mapped polygon.  This
//// routine uses affine texture-mapping between sub-spans of SUB_SPAN length
//// while only performing perspective correction every SUB_SPAN pixels.  This
//// produces a much faster routine that the one above, but suffers from accuracy
//// loss.
////
//// This routine also suffers from other aliasing problems of the first two,
//// however, since these polygons are an estimated perspective-correct, they
//// choose texels in a non-perfect fasion. Remember that the perspective curve
//// (explained in the comments above the previous example) is being estimated
//// with linear interpolation (i.e. straight lines.)  This can cause the error
//// (already present in a non-linear estimation) to be amplified even more.
////
//// The greater the SUB_SPAN length, the less "perspective correction" is
//// performed AND the less accurately texels will be chosen.
////
//// This routine also uses a fixed-point representation of the UV values as it
//// interpolates each sub-span.  This should not cause any problems since the
//// fixed-point representation is 8.24 (24 bits used to represent the fractional
//// component) which is a higher degree of resolution than a 32-bit floating-
//// point variable offers.  However, if the delta from texel to texel goes
//// beyond 255.999... texels from texel to texel, the value will overflow and
//// results may be unpredictable.
//// ----------------------------------------------------------------------------
//
//void	drawSubPerspectiveTexturedPolygon(sVERT *verts)
//{
//	// Find the top-most vertex
//
//	sVERT	*v = verts, *lastVert = verts, *lTop = verts, *rTop;
//
//	while(v)
//	{
//		if (v->y < lTop->y) lTop = v;
//		lastVert = v;
//		v->iy = ceil(v->y);
//		v = v->next;
//	}
//
//	rTop = lTop;
//
//	// Top scanline of the polygon in the frame buffer
//
//	char	*fb = &backBuffer[lTop->iy * RES_X];
//
//	// Left & Right edges (primed with 0)
//
//	sEDGE	le, re; le.height = 0; re.height = 0;
//
//	// Render the polygon
//
//	while(1)
//	{
//		if (!le.height)
//		{
//			sVERT	*lBot = lTop - 1; if (lBot < verts) lBot = lastVert;
//			le.height = lBot->iy - lTop->iy;
//			if (le.height < 0) return;
//			calcEdgeDeltas(le, lTop, lBot);
//			lTop = lBot;
//		}
//
//		if (!re.height)
//		{
//			sVERT	*rBot = rTop + 1; if (rBot > lastVert) rBot = verts;
//			re.height = rBot->iy - rTop->iy;
//			if (re.height < 0) return;
//			calcEdgeDeltas(re, rTop, rBot);
//			rTop = rBot;
//		}
//
//		// Polygon must have height
//
//		if (!le.height && !re.height) return;
//
//		// Get the height
//
//		int	height = MIN(le.height, re.height);
//
//		// Subtract the height from each edge
//
//		le.height -= height;
//		re.height -= height;
//
//		// Render the current trapezoid defined by left & right edges
//
//		while(height-- > 0)
//		{
//			// Texture coordinates
//
//			float	overWidth = 1.0 / (re.x - le.x);
//			float	du = (re.u - le.u) * overWidth;
//			float	dv = (re.v - le.v) * overWidth;
//			float	dw = (re.w - le.w) * overWidth;
//
//			// Find the end-points
//
//			int	start = (int) ceil(le.x);
//			int	end   = (int) ceil(re.x);
//
//			// Texture adjustment (some call this "sub-texel accuracy")
//
//			float	subTex = (float) start - le.x;
//			float	u = le.u + du * subTex;
//			float	v = le.v + dv * subTex;
//			float	w = le.w + dw * subTex;
//
//			// Start of the first span
//
//			float	z  = 1.0 / w;
//			float	s1 = u * z;
//			float	t1 = v * z;
//
//			// Fill the entire span
//
//			char	*span = fb + start;
//			int	pixelsDrawn = 0;
//
//			for(; start < end; start += SUB_SPAN)
//			{
//				// Start of the current span
//
//				float	s0 = s1;
//				float	t0 = t1;
//
//				int	len = MIN(SUB_SPAN, end - start);
//				pixelsDrawn += len;
//
//				// End of the current span
//
//				z  = 1.0 / (w + dw * pixelsDrawn);
//				s1 = z *   (u + du * pixelsDrawn);
//				t1 = z *   (v + dv * pixelsDrawn);
//
//				// The span (8.24 fixed-point)
//
//				float		divisor = 1.0 / len * 0x1000000;
//				unsigned int	ds = (s1 - s0) * divisor;
//				unsigned int	dt = (t1 - t0) * divisor;
//				unsigned int	s  = s0 * 0x1000000;
//				unsigned int	t  = t0 * 0x1000000;
//
//				// Draw the sub-span
//
//				for (int j = 0; j < len; j++)
//				{
//					*(span++) += textureBuffer[((t>>18)&0xffffffC0)+(s>>24)];
//					s += ds;
//					t += dt;
//				}
//			}
//
//			// Scanline step
//
//			le.u += le.du;
//			le.v += le.dv;
//			le.w += le.dw;
//			le.x += le.dx;
//			re.u += re.du;
//			re.v += re.dv;
//			re.w += re.dw;
//			re.x += re.dx;
//			fb += RES_X;
//		}
//	}
//}
//
//// ----------------------------------------------------------------------------
//// Silly little test routine to test the polygon renderers
//// ----------------------------------------------------------------------------
//
//void	main(void)
//{
//	// Initialize our texture buffer
//
//	drawTexture();
//
//	// Set the mode
//
//	setMode(0x13);
//
//	// Setup the 4 adjacent polygons
//
//	sVERT	p0[4];
//	p0[0].x = -1.0; p0[0].y = -1.0; p0[0].z =  1.0; p0[0].next = &p0[1];
//	p0[1].x =    0; p0[1].y = -1.0; p0[1].z =  1.0; p0[1].next = &p0[2];
//	p0[2].x =    0; p0[2].y =    0; p0[2].z =    0; p0[2].next = &p0[3];
//	p0[3].x = -1.0; p0[3].y =    0; p0[3].z =    0; p0[3].next = NULL;
//
//	sVERT	p1[4];
//	p1[0].x =    0; p1[0].y = -1.0; p1[0].z =  1.0; p1[0].next = &p1[1];
//	p1[1].x =  1.0; p1[1].y = -1.0; p1[1].z =  1.0; p1[1].next = &p1[2];
//	p1[2].x =  1.0; p1[2].y =    0; p1[2].z =    0; p1[2].next = &p1[3];
//	p1[3].x =    0; p1[3].y =    0; p1[3].z =    0; p1[3].next = NULL;
//
//	sVERT	p2[4];
//	p2[0].x = -1.0; p2[0].y =    0; p2[0].z =    0; p2[0].next = &p2[1];
//	p2[1].x =    0; p2[1].y =    0; p2[1].z =    0; p2[1].next = &p2[2];
//	p2[2].x =    0; p2[2].y =  1.0; p2[2].z = -1.0; p2[2].next = &p2[3];
//	p2[3].x = -1.0; p2[3].y =  1.0; p2[3].z = -1.0; p2[3].next = NULL;
//
//	sVERT	p3[4];
//	p3[0].x =    0; p3[0].y =    0; p3[0].z =    0; p3[0].next = &p3[1];
//	p3[1].x =  1.0; p3[1].y =    0; p3[1].z =    0; p3[1].next = &p3[2];
//	p3[2].x =  1.0; p3[2].y =  1.0; p3[2].z = -1.0; p3[2].next = &p3[3];
//	p3[3].x =    0; p3[3].y =  1.0; p3[3].z = -1.0; p3[3].next = NULL;
//
//	sVERT	*polys[4] = {p0, p1, p2, p3};
//	int	polyCount = 4;
//
//	float	theta = 0.0;
//
//	// For the timer
//
//	double	totalTicks = 0.0, totalPolygons = 0.0;
//
//	// Animate
//
//	float	speed = 30.0;
//
//	while(!kbhit() && totalPolygons < POLY_COUNT)
//	{
//		// Clear the backBuffer
//
//		memset(backBuffer, 0, sizeof(backBuffer));
//
//		// Rotate just a little
//
//		theta   += 0.0003 * speed;
//
//		// Draw the polygons
//
//		for (int i = 0; i < polyCount; i++)
//		{
//			// Temporary polygon
//
//			sVERT	poly[4];
//
//			// Offset/scale the vertices
//
//			sVERT	*src = polys[i];
//			sVERT	*dst = poly;
//
//			while(src)
//			{
//				// Rotate
//
//				dst->u = src->x * (0.49 * TEX_X) + (0.5 * TEX_X);
//				dst->v = src->y * (0.49 * TEX_Y) + (0.5 * TEX_Y);
//				dst->w = 1.0;
//				dst->x = src->x * cos(theta) - src->y * sin(theta);
//				dst->y = src->x * sin(theta) + src->y * cos(theta);
//				dst->z = src->z;
//
//				// Scale
//
//				dst->x *= 700.0;
//				dst->y *= 700.0;
//				dst->z *= 10.0; dst->z += 20.0;
//
//				// Project
//
//				#ifndef USE_AFFINE
//				dst->u /= dst->z;
//				dst->v /= dst->z;
//				dst->w /= dst->z;
//				#endif
//				dst->x /= dst->z;
//				dst->y /= dst->z;
//
//				// Offset to screen center
//
//				dst->x += RES_X / 2.0 + 0.5;
//				dst->y += RES_Y / 2.0 + 0.5;
//
//				// Terminate the list
//
//				dst->next = src->next ? &dst[1] : NULL;
//
//				// Next!
//
//				src = src->next;
//				dst = dst->next;
//			}
//
//			// Time the drawing
//
//			unsigned int	sHi, sLo, eHi, eLo;
//			getTicks(&sHi, &sLo);
//
//			#ifdef USE_AFFINE
//			drawAffineTexturedPolygon(poly);
//			#endif
//
//			#ifdef USE_EXACT_PERSPECTIVE
//			drawPerspectiveTexturedPolygon(poly);
//			#endif
//
//			#ifdef USE_SUB_AFFINE_PERSPECTIVE
//			drawSubPerspectiveTexturedPolygon(poly);
//			#endif
//
//			// Calculate ticks
//
//			getTicks(&eHi, &eLo);
//			double	highMultiplier = pow(2.0, 32.0);
//			double	sTicks = (double) sHi * highMultiplier + (double) sLo;
//			double	eTicks = (double) eHi * highMultiplier + (double) eLo;
//			totalTicks  += eTicks - sTicks;
//			totalPolygons += 1.0;
//		}
//
//		// Copy to the display
//
//		memcpy(frameBuffer, backBuffer, sizeof(backBuffer));
//	}
//
//	// Get the pending key
//
//	if (totalPolygons < POLY_COUNT) getch();
//
//	// Restore text mode
//
//	setMode(3);
//
//	// Print the timing information
//
//	printf("%d average ticks per polygon (%d polygons drawn).\n", (int) (totalTicks / totalPolygons), (int) totalPolygons);
//}
//
//// ----------------------------------------------------------------------------
//// Tmap.cpp - End of file
//// ----------------------------------------------------------------------------
//
//#endif //0

*/
