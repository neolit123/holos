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
#ifndef h_Base_included
#define h_Base_included
//----------------------------------------------------------------------

#include "lib/h_Rect.h"
#include "core/h_Parameter.h"

//----------------------------------------------------------------------

class h_Host_Base
{
  public:
    h_Host_Base() {}
    virtual ~h_Host_Base() {}
};

//----------

class h_Descriptor_Base
{
  public:
    const char* m_Name;
    const char* m_Author;
    const char* m_Product;
    int         m_Flags;
    h_Rect      m_EditorRect;
};

//----------

class h_Instance_Base
{
  public:
    h_Instance_Base(h_Descriptor_Base* a_Descriptor) {}
    virtual ~h_Instance_Base() {}
    virtual void do_HandleState(int a_State) {}
    virtual void do_HandleTransport(int a_Value) {}
    virtual void do_HandleMidi(int a_Offset, unsigned char aMsg1, unsigned char aMsg2, unsigned char aMsg3) {}
    virtual void do_HandleParameter(h_Parameter* a_Parameter) {}
    virtual bool do_ProcessBlock(float** a_Inputs, float** a_Outputs, int a_Length) { return false; }
    virtual void do_ProcessSample(float** a_Inputs, float** a_Outputs) {}
    virtual void do_PostProcess(float** a_Inputs, float** a_Outputs, int a_Length) {}
    virtual void do_OpenEditor(void* a_Parent) {}
    virtual void do_CloseEditor() {}
    virtual void do_IdleEditor(void) {}
};

//----------

class h_Editor_Base
{
  public:
    h_Editor_Base(h_Instance_Base* a_Instance, h_Rect a_Rect/*, void* a_Parent*/) {}
    virtual ~h_Editor_Base() {}
    virtual void do_Open(void* a_Ptr) {} // h_Window*
    virtual void do_Close(void) {}
    virtual void do_EventLoop(void) {}
    //virtual void on_Change(h_Widget* a_Widget) {}
};

//----------------------------------------------------------------------

struct h_Platform_Data;
struct h_Interface_Data;
//struct h_Format_Data;

//----------------------------------------

class h_Platform_Base
{
  public:
    h_Platform_Base() {}
    virtual ~h_Platform_Base() {}
    virtual h_Platform_Data* getData(void) { return H_NULL; }
};

//----------

class h_Interface_Base
{
  public:
    h_Interface_Base() {}
    virtual ~h_Interface_Base() {}
    virtual h_Interface_Data* getData(void) { return H_NULL; }
};

//----------

class h_Format_Base
{
  public:
    h_Format_Base() {}
    virtual ~h_Format_Base() {}
    virtual void* entrypoint(h_Host_Base* a_Host) { return H_NULL; }
    //virtual h_Format_Data* getData(void) { return H_NULL; }
};

//----------------------------------------------------------------------
#endif
