// Compiled as :
// cl /DWIN32 sample.cpp yspng.cpp kernel32.lib user32.lib gdi32.lib advapi32.lib opengl32.lib glu32.lib glaux.lib


#ifdef WIN32
#include <windows.h>
#else
#define CALLBACK
#endif


#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

#include <stdio.h>

#include "yspng.h"


static int winWid,winHei;
YsRawPngDecoder png;

extern "C" void CALLBACK DisplayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	if(png.wid>0 && png.hei>0 && png.rgba!=NULL)
	{
		if(png.hei<=winHei)
		{
			glRasterPos2i(1,png.hei-1);
		}
		else
		{
			glRasterPos2i(1,winHei-1);
		}
		glDrawPixels(png.wid,png.hei,GL_RGBA,GL_UNSIGNED_BYTE,png.rgba);
	}
}

extern "C" void CALLBACK ReshapeFunc(GLsizei x,GLsizei y)
{
	winWid=x;
	winHei=y;
	DisplayFunc();
}

int main(int ac,char *av[])
{
	png.Decode(av[1]);
	png.Flip();   // Need to flip upside down because glDrawPixels draws y=0 bottom.

	auxInitDisplayMode(AUX_SINGLE|AUX_RGB);
	auxInitPosition(0,0,png.wid+2,png.hei+2);
	auxInitWindow(NULL);

	winWid=png.wid;
	winHei=png.hei;


	glViewport(0,0,png.wid,png.hei);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,(double)viewport[2],(double)viewport[3],0.0,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	auxReshapeFunc(ReshapeFunc);
	auxMainLoop(DisplayFunc);

	return 0;
}
