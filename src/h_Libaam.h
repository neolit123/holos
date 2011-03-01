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
#ifndef h_Libaam_included
#define h_Libaam_included
//----------------------------------------------------------------------

//#include "lib/h_Defines.h"
//#include "lib/h_String.h"
#include "src/h_Defines.h"
#include "src/h_String.h"

//----------

#ifdef H_LINUX
  #include <dlfcn.h>
#endif

#ifdef H_WIN32
  #include <windows.h>
#endif

//----------------------------------------------------------------------

#define libaam_STREAM_INIT         0
#define libaam_STREAM_OPEN         1
#define libaam_STREAM_CLOSE        2
//#define libaam_STREAM_PLAY         3
//#define libaam_STREAM_STOP         4
#define libaam_STREAM_EXIT         5
#define libaam_STREAM_ABOUT        6
#define libaam_STREAM_AUDIO_COUNT  7
#define libaam_STREAM_AUDIO_NAME   8
#define libaam_STREAM_FRAMES       9
#define libaam_STREAM_RATE         10
#define libaam_STREAM_LATENCY      17
#define libaam_STREAM_RELOAD       18
#define libaam_MIDI_IN_COUNT       30
#define libaam_MIDI_IN_NAME        31
#define libaam_MIDI_IN_ENABLE      32
#define libaam_MIDI_IN_ENABLED     33
#define libaam_AUDIO_IN_COUNT      40
#define libaam_AUDIO_IN_NAME       41
#define libaam_AUDIO_IN_ENABLE     42
#define libaam_AUDIO_IN_ENABLED    43
#define libaam_AUDIO_OUT_COUNT     50
#define libaam_AUDIO_OUT_NAME      51
#define libaam_AUDIO_OUT_ENABLE    52
#define libaam_AUDIO_OUT_ENABLED   53

//----------

typedef int (*libaam_dispatcher)(int index, int value1, int value2) asm ("libaam");
typedef int (*libaam_process)(float** inputs, float** outputs, int numins, int numouts, int frames, void* mididata, int mididatacount);

//----------------------------------------------------------------------

class h_Libaam
{
  private:

    bool              mLoaded;
    void*             mHandle;
    libaam_dispatcher mDispatcher;

  public:

    h_Libaam()
      {
        mLoaded     = false;
        mHandle     = NULL;
        mDispatcher = NULL;
      }

    //----------

    virtual ~h_Libaam()
      {
        close();
      }

    inline bool loaded(void) { return mLoaded; }

    //----------------------------------------
    // dll
    //----------------------------------------

    // return:
    //  0 OK
    // -1 Already loaded
    // -2 Library (dll/so) not found
    // -3 Dispatcher function not found in library
    // -4 Unknown

    int load(h_String aFileName)
      {
        char temp[H_MAX_STRINGSIZE];
        h_Strcpy(temp,aFileName.ptr());
        #ifdef H_LINUX
          h_Strcat(temp,".so");
          mHandle = dlopen(/*aFileName.ptr()*/temp,RTLD_NOW/*RTLD_LAZY*/);  // crashed if 0
          //trace("mHandle: " << mHandle);
          if (!mHandle) return -2;
          mDispatcher = (libaam_dispatcher)dlsym(mHandle,(char*)"libaam");
          if (!mDispatcher) return -3;
        #endif
        #ifdef H_WIN32
          //mLoaded = false;
          //return -4;
          h_Strcat(temp,".dll");
          mHandle = LoadLibrary(temp);
          if (!mHandle) return -2;
          mDispatcher = (libaam_dispatcher)GetProcAddress((HINSTANCE)mHandle,"libaam");
          if (!mDispatcher) return -3;
        #endif
        mLoaded = true;
        return 0;
      }

    //----------

    int close(void)
      {
        #ifdef H_LINUX
          if (mHandle) dlclose(mHandle);
        #endif
        #ifdef H_WIN32
        #endif
        mLoaded = false;
        mHandle = NULL;
        mDispatcher = NULL;
        return 0;
      }

    //----------

    int dispatch(int aIndex, int aValue1, int aValue2)
      {
        if (!mDispatcher) return -1;
        return mDispatcher(aIndex,aValue1,aValue2);
      }

    //----------------------------------------
    // stream
    //----------------------------------------

    // init library
    int streamInit(libaam_process process)
      {
        //case STREAM_INIT:
        //  process = (CProcess*)value1;
        return mDispatcher(libaam_STREAM_INIT,(int)process,0);   // !!! 32-bit (int)
      }

    //----------

    // open device
    int streamOpen(int num)
      {
        //case STREAM_OPEN:
        //  result = 1;
        return mDispatcher(libaam_STREAM_OPEN,num,0);
      }

    //----------

    // close device
    int streamClose(void)
      {
        //case STREAM_CLOSE:
        //  result = 1;
        return mDispatcher(libaam_STREAM_CLOSE,0,0);
      }

    //----------

    // libaam_STREAM_PLAY
    // libaam_STREAM_STOP

    //----------

    // exit library
    int streamExit(void)
      {
        //case STREAM_EXIT:
        return mDispatcher(libaam_STREAM_EXIT,0,0);
      }

    //----------

    // libaam_STREAM_ABOUT

    //----------

    // device count
    int streamAudioCount(void)
      {
        //case STREAM_AUDIO_COUNT:
        //  result = 1; // jack only
        return mDispatcher(libaam_STREAM_AUDIO_COUNT,0,0);
      }

    //----------

    // device name
    int streamAudioName(int num, char* str)
      {
        //case STREAM_AUDIO_NAME:
        //  strcpy((char*)value2, "JACK Audio");
        return mDispatcher(libaam_STREAM_AUDIO_NAME,num,(int)str);   // !!! 32-bit
      }

    //----------

    // get/set frames
    int streamFrames(int frames=0)
      {
        //case STREAM_FRAMES:
        //  if (value1 > 0) frames = value1;
        //  result = frames;
        return mDispatcher(libaam_STREAM_FRAMES,frames,0);
      }

    //----------

    // get/set rate
    int streamRate(int setRate, float* getRate)
      {
        //case STREAM_RATE:
        //  if (value1 == 0) *(float*)value2 = rate;
        //  else if (value1 > 0) rate = value1;
        return mDispatcher(libaam_STREAM_RATE,setRate,(int)getRate); // !!!
      }

    //----------

    // STREAM_LATENCY
    // STREAM_RELOAD

    //----------------------------------------
    // midi
    //----------------------------------------

    // midi in count
    int midiInCount(void)
      {
        //case MIDI_IN_COUNT:
        //  return midiDevices;
        return mDispatcher(libaam_MIDI_IN_COUNT,0,0);
      }

    //----------

    // midi in name
    int MidiInName(int num, char* str)
      {
        //case MIDI_IN_NAME:
        //  if (value1 >= 0 && value1 < midiDevices)
        //    strcpy((char*)value2, mdevs[value1]->id);
        return mDispatcher(libaam_MIDI_IN_NAME,num,(int)str);   // !!!
      }

    //----------

    // enable midi in
    int MidiInEnable(int num, int enabled)
      {
        //case MIDI_IN_ENABLE:
        //  if (value1 >= 0 && value1 < midiDevices) mdevs[value1]->enable(value2);
        return mDispatcher(libaam_MIDI_IN_ENABLE,num,enabled);
      }

    //----------

    // is midi in enabled?
    int MidiInEnabled(int num)
      {
        //case MIDI_IN_ENABLED:
        //  if (value1 >= 0 && value1 < midiDevices) return mdevs[value1]->enabled;
        return mDispatcher(libaam_MIDI_IN_ENABLED,num,0);
      }

    //----------------------------------------
    // audio in
    //----------------------------------------

    // audio in count
    int AudioInCount(void)
    {
      //case AUDIO_IN_COUNT:
      //  return aincount;
      return mDispatcher(libaam_AUDIO_IN_COUNT,0,0);
    }

    //----------

    // audio in name
    int AudioInName(int num, char* str)
    {
      //case AUDIO_IN_NAME:
      //  if (value1 >= 0 && value1 < aincount)
      //    strcpy((char*)value2, ain[value1].id);
      return mDispatcher(libaam_AUDIO_IN_NAME,num,(int)str);   // !!!
    }

    //----------

    // audio out enable
    int AudioInEnable(int num, int enabled)
    {
      //case AUDIO_IN_ENABLE:
      //  if (value1 >= 0 && value1 < maxChs)
      //    ain[value1].enabled = value2;
      return mDispatcher(libaam_AUDIO_IN_ENABLE,num,enabled);
    }

    //----------

    // audio in enabled
    int AudioInEnabled(int num)
    {
      //case AUDIO_IN_ENABLED:
      //  if (value1 >= 0 && value1 < maxChs)
      //    result = ain[value1].enabled;
      return mDispatcher(libaam_AUDIO_IN_ENABLED,num,0);
    }

    //----------------------------------------
    // audio out
    //----------------------------------------

    // audio out count
    int AudioOutCount(void)
    {
      //case AUDIO_OUT_COUNT:
      //  return aoutcount;
      return mDispatcher(libaam_AUDIO_OUT_COUNT,0,0);
    }

    //----------

    // audio out name
    int AudioOutName(int num, char* str)
    {
      //case AUDIO_OUT_NAME:
      //  if (value1 >= 0 && value1 < aoutcount)
      //    strcpy((char*)value2, aout[value1].id);
      return mDispatcher(libaam_AUDIO_OUT_NAME,num,(int)str);    // !!!
    }

    //----------

    // audio out enable
    int AudioOutEnable(int num, int enable)
    {
      //case AUDIO_OUT_ENABLE:
      //  if (value1 >= 0 && value1 < maxChs)
      //    aout[value1].enabled = value2;
      return mDispatcher(libaam_AUDIO_OUT_ENABLE,num,enable);
    }

    //----------

    // audio out enabled
    int AudioOutEnabled(int num)
    {
      //case AUDIO_OUT_ENABLED:
      //  if (value1 >= 0 && value1 < maxChs)
      //    result = aout[value1].enabled;
      return mDispatcher(libaam_AUDIO_OUT_ENABLED,num,0);
    }

};

//----------------------------------------------------------------------
#endif

/*

based on sources from http://svn.jacklab.net
and http://www.energy-xt.com/download/jack.cpp

you need to get a libaam.so/dll from
http://www.energy-xt.com/xt2.php
(or evenyually compile your own .so for linux)

http://www.kvraudio.com/forum/viewtopic.php?t=217002&postdays=0&postorder=asc&highlight=libaam&start=15

nexxusix:   "can the version of jack.cpp you have be used for other applications?"
drumfix:    "to my knowledge the code is public domain, so you can use it in your
             own projects. in general all your app has to do is to call procedure
             libaam with the appropriate dispatcher codes. if you understand how
             this procedure works you can easily add midiout as well."

http://www.energy-xt.com/forum/index.php?action=posts&forumId=2&topicId=453

jorgen:     "ok, heres the source code for using jack with XT2."
metasymbol: "under what licence you publicing your open source software?"
jorgen:     "the source is public domain"

- jack.cpp
http://www.energy-xt.com/download/jack.php
http://people.jacklab.net/drumfix/jack.cpp
http://svn.jacklab.net/listing.php?repname=eXT2&path=%2Flibaam%2F#_libaam_

- energy-xt & libaam
http://www.energy-xt.com/
http://www.energy-xt.com/xt2.php

*/

