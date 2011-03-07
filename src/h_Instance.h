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
#ifndef h_Instance_included
#define h_Instance_included
//----------------------------------------------------------------------

//#include "core/h_Host.h"
//#include "core/h_Parameter.h"
//#include "core/h_Descriptor.h"
#include "src/h_Host.h"
#include "src/h_Parameter.h"
#include "src/h_Descriptor.h"

//----------------------------------------------------------------------

// instance states
#define is_None     0
#define is_Open     1
#define is_Close    2
#define is_Suspend  3
#define is_Resume   4
#define is_Rate     5
#define is_Block    6

//----------

// instance transport
#define ft_None         0   // indicates that play, cycle or record state has changed
#define ft_Changed      1   // indicates that play, cycle or record state has changed
#define ft_Playing      2   // set if Host sequencer is currently playing
#define ft_CycleActive  4   // set if Host sequencer is in cycle mode
#define ft_Recording    8   // set if Host sequencer is in record mode
#define ft_AutoWriting  64  // set if automation write mode active (record parameter changes)
#define ft_AutoReading  128 // set if automation read mode active (play parameter changes)

//----------------------------------------------------------------------

class h_Instance_Base
{
  public:
    virtual void    do_HandleState(int a_State) {}
    virtual void    do_HandleTransport(int a_State) {}
    virtual void    do_HandleParameter(h_Parameter* a_Parameter) {}
    virtual void    do_HandleMidi(int a_Offset, unsigned char a_Msg1, unsigned char a_Msg2, unsigned char a_Msg3) {}
    virtual bool    do_ProcessBlock(float** a_Inputs, float** a_Outputs, int a_Length) { return false; }
    virtual void    do_ProcessSample(float** a_Inputs, float** a_Outputs) {}
    virtual void    do_PostProcess(float** a_Inputs, float** a_Outputs, int a_Length) {}
    virtual void*   do_OpenEditor(void* ptr) { return H_NULL; } // return h_Editor
    virtual void    do_CloseEditor(void) {}
    virtual void    do_IdleEditor(void) {}
    virtual void    do_PreProgram(int a_Index) {}
    virtual void    do_SetProgram(int a_Index) {}
  public:
    virtual int     getPlayState(void)      { return 0; }
    virtual double  getSamplePos(void)      { return 0; }
    virtual double  getSampleRate(void)     { return 0; }
    virtual double  getBeatPos(void)        { return 0; }
    virtual double  getTempo(void)          { return 0; }
    virtual int     getCurrentProgram(void) { return 0; }
  public:
    virtual void    transferParameters(void) {}
    virtual void    transferProgram(int a_Index) {}
    virtual void    saveProgram(int a_Index) {}
    virtual void    notifyParameter(h_Parameter* a_Parameter) {}
    virtual void    notifyResize(int aWidth, int aHeight) {}
    virtual void    updateTime(void) {}
    virtual void    sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3) {}
};

//----------------------------------------------------------------------

#ifdef H_EXE
  #include "src/h_Instance_Exe.h"
#endif

#ifdef H_LADSPA
  #include "src/h_Instance_Ladspa.h"
#endif

#ifdef H_VST
  #include "src/h_Instance_Vst.h"
#endif

//----------------------------------------------------------------------
#endif
