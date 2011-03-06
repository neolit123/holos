/*
  test_ico.cpp

  cd /holos/build
  windres -i rc_default.rc -o rc_default.rc.o
  g++ ./test/test_ico.cpp ./rc_default.rc.o -o ./test_ico.exe
*/

#include "windows.h"

const char g_szClassName[] = "someWindowClass";

LRESULT CALLBACK WndProc( HWND Hwnd,
                          UINT Msg,
                          WPARAM wParam,
                          LPARAM lParam)
{
  switch(Msg)
  {
    case WM_CLOSE:
      DestroyWindow(Hwnd);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(Hwnd, Msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    INT nCmdShow)
{
  WNDCLASS wc;
  HWND Hwnd;
  MSG Msg;

  memset(&wc, 0, sizeof(wc));

  wc.lpfnWndProc    = WndProc;
  wc.hInstance      = hInstance;
  wc.lpszClassName  = g_szClassName;
  wc.hIcon          = LoadIcon(hInstance, "h_icon");
  wc.hbrBackground  = (HBRUSH)(COLOR_WINDOW);

  LONG rcResult = RegisterClass(&wc);

  Hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                        g_szClassName,
                        g_szClassName,
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        400,
                        400,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

  ShowWindow(Hwnd, nCmdShow);

  while(GetMessage(&Msg, NULL, 0, 0) > 0)
  {
    DispatchMessage(&Msg);
  }

  return Msg.wParam;
}
