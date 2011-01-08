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
#ifndef h_VoiceManager_included
#define h_VoiceManager_included
//----------------------------------------------------------------------

//#include ""

class h_VoiceManager //: public h_
{
  public:
    h_VoiceManager() /*: h_()*/ {}
    ~h_VoiceManager() {}
};

//----------------------------------------------------------------------
#endif
























//
///*
// * This file is part of Axonlib.
// *
// * Axonlib is free software: you can redistribute it and/or modify
// * it under the terms of the Axonlib License, either version 1.0
// * of the License, or (at your option) any later version.
// *
// * Axonlib is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// * See LICENSE_AX for more details.
// *
// * You should have received a copy of the Axonlib License
// * If not, see <http://axonlib.googlecode.com/>.
// */
//
///**
//  \file axVoices.h
//  \brief polyphonic voice manager
// */
//
//#ifndef axVoices_included
//#define axVoices_included
////----------------------------------------------------------------------
//
//// - max block size: 999999 samples
////     a biiig offset that we will hopefully never reach
//// - max events per block: #define MAX_EVENTS 1024
////     a static buffer for the incoming events, to avoid runtime memory juggling
//// - very basic nna/note-stealing (works ok-ish)
//// - currently only note on/off
//// - converts events in processSample
////     could be done in processBlock (does it matter?)
//// - not optimized
////     but 16 basic saw voices uses only around 0.8% cpu (reaper/wine)
////     so i've postponed that until it gets problematic
//// - bugs?
//
//#include "core/axList.h"
//
////----------------------------------------------------------------------
////
//// voice
////
////----------------------------------------------------------------------
//
//// voice states
//#define vs_Off       0
//#define vs_Playing   1
//#define vs_Released  2
//
////----------
//
//class axVoice : public axListNode
//{
//  friend class axVoiceManager;
//  protected:
//    int   mState;
//    float iRate;
//  public:
//    axVoice() { mState = vs_Off; }
//    virtual ~axVoice() {}
//    virtual void  setSampleRate(float aRate) { iRate = 1/aRate; }
//    virtual void  noteOn(int aNote, int aVel) {}
//    virtual void  noteOff(int aNote, int aVel) {}
//    virtual void  ctrlChange(int aCtrl, int aVal) {}
//    virtual void  control(int aIndex, float aVel) {}
//    //virtual float process(void) {return 0;}
//    virtual void  process(float* outs) {}
//};
//
//typedef axArray<axVoice*> axVoices;
//
////----------------------------------------------------------------------
////
//// voice manager
////
////----------------------------------------------------------------------
//
///*
//
//[assume events in buffer are sorted by their offset in the audiobuffer]
//
//we have:
//- an array with all available voices (allocated at startup)
//- a list of available voices (initially containing ptr to all available voices)
//- a list of playing voices
//- a list of released (decaying) voices
//note on:
//- grab a voice from the free voices list, and append it to the playing list
//- if no voice available in the free list, take the oldest voice from decaying voices
//note off:
//- move the voice from the playing to the released list
//process:
//- process both playing and released lists,
//  call process for each voice, and add their outputs
//  voices can set their mState to vs_Off to shut themselves off (see post-process)
//post-process:
//- move all voices with mState == vs_Off from released to free voices list
//- cleanup buffers and counters
//
//midi:
//we copy all incoming midi events to a buffer.
//during sample processing:
//keep track of current offset (position in buffer), and next event to process
//when offset reaches next event, we fire off note on/ff etc events
//and continue, looking at the next event and its offset
//
//
//after the block has finished, we reset the buffers and offset
//and prepare for next block
//
//*/
//
//struct vm_event
//{
//  int ofs;
//  unsigned char msg1,msg2,msg3;
//  char padding;
//};
//
//#define MAX_EVENTS 1024
//
////----------------------------------------
//
//class axVoiceManager
//{
//  private:
//    int       mOffset;
//    int       mNextEvent;
//    int       mCurrEvent;
//    int       mNumEvents;
//    vm_event  mEvents[MAX_EVENTS];
//    int       mNumPlaying;
//
//  protected:
//    axVoice*  mNoteMap[128];
//    axVoices  mAllVoices;
//    axList    mFreeVoices;
//    axList    mPlayingVoices;
//    axList    mReleasedVoices;
//  public:
//
//    axVoiceManager()
//      {
//        axMemset(mNoteMap,0,sizeof(mNoteMap));
//        mOffset = 0;
//        mCurrEvent = 0;
//        mNextEvent = 999999;
//        mNumEvents = 0;
//        axMemset(mEvents,0,sizeof(mEvents));
//        mNumPlaying = 0;
//      }
//
//    //----------
//
//    virtual ~axVoiceManager()
//      {
//        #ifndef AX_NOAUTODELETE
//        deleteVoices();
//        #endif
//      }
//
//    //----------------------------------------
//
//    virtual void appendVoice(axVoice* V)
//      {
//        mAllVoices.append(V);
//        mFreeVoices.append(V);
//      }
//
//    //----------
//
//    virtual void deleteVoices(void)
//      {
//        for (int i=0; i<mAllVoices.size(); i++)
//          delete mAllVoices[i];
//      }
//
//    //----------
//
//    virtual void setSampleRate( float aRate )
//      {
//        for (int i=0; i<mAllVoices.size(); i++)
//        {
//          mAllVoices[i]->setSampleRate(aRate);
//        }
//      }
//
//    //----------------------------------------
//
//    virtual void noteOn(int aNote, int aVel)
//      {
//        axVoice* V = (axVoice*)mFreeVoices.getTail();
//        if (V) { mFreeVoices.removeTail(); }
//        else
//        {
//          V = (axVoice*)mReleasedVoices.getHead();
//          if (V) { mReleasedVoices.removeHead(); }
//        }
//        if (V)
//        {
//          mNoteMap[aNote] = V;
//          mPlayingVoices.append(V);
//          V->mState = vs_Playing;
//          V->noteOn(aNote,aVel);
//          mNumPlaying++;
//        }
//      }
//
//    //----------
//
//    virtual void noteOff(int aNote, int aVel)
//      {
//        axVoice* V = mNoteMap[aNote];
//        if (V)
//        {
//          mNoteMap[aNote] = NULL;
//          mPlayingVoices.remove(V);
//          mReleasedVoices.append(V);
//          V->mState = vs_Released;
//          V->noteOff(aNote,aVel);
//          mNumPlaying--;
//        }
//      }
//
//    //----------
//
//    void control(int aIndex, float aVal)
//      {
//        for (int i=0; i<mAllVoices.size(); i++) mAllVoices[i]->control(aIndex,aVal);
//      }
//
//    //----------
//
//    void midi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
//      {
//        //trace(" ofs "<<ofs << " msg1 "<<msg1 << " msg2 "<<msg2 << " msg3 "<<msg3);
//        if (mNumEvents<MAX_EVENTS)
//        {
//          mEvents[mNumEvents].ofs  = ofs;
//          mEvents[mNumEvents].msg1 = msg1;
//          mEvents[mNumEvents].msg2 = msg2;
//          mEvents[mNumEvents].msg3 = msg3;
//          mNumEvents++;
//        }
//      }
//
//    //----------
//
//    void preProcess(void)
//      {
//        mOffset = 0;
//        mCurrEvent = 0;
//        if (mNumEvents>0) mNextEvent = mEvents[0].ofs;
//        else mNextEvent = 999999;
//      }
//
//    //----------
//
//    // stereo
//    void process(float* outs)
//      {
//        // events
//        while (mOffset==mNextEvent)
//        {
//          //int chn  = mEvents[mCurrEvent].msg1 & 0x0f;
//          int msg  = mEvents[mCurrEvent].msg1 & 0xf0;
//          int note = mEvents[mCurrEvent].msg2;
//          int vel  = mEvents[mCurrEvent].msg3;
//          switch(msg)
//          {
//            case 0x90:
//              if (vel==0) noteOff(note,vel);
//              else noteOn(note,vel);
//              break;
//            case 0x80:
//              note = mEvents[mCurrEvent].msg2;
//              vel  = mEvents[mCurrEvent].msg3;
//              noteOff(note,vel);
//              break;
//          }
//          mCurrEvent++;
//          if (mCurrEvent<mNumEvents) mNextEvent = mEvents[mCurrEvent].ofs;
//          else mNextEvent = 999999;
//        }
//        mOffset++;
//        //float out = 0;
//        float left = 0;
//        float right = 0;
//        float _outs[2];
//        // playing voices
//        axVoice* V = (axVoice*)mPlayingVoices.getHead();
//        while (V)
//        {
//          //out += V->process();
//          V->process(&_outs[0]);
//          left += _outs[0];
//          right += _outs[1];
//          V = (axVoice*)V->getNext();
//        }
//        // released voices
//        V = (axVoice*)mReleasedVoices.getHead();
//        while (V)
//        {
//          if (V->mState!=vs_Off) //out += V->process();
//          {
//            V->process(&_outs[0]);
//            left += _outs[0];
//            right += _outs[1];
//          }
//          V = (axVoice*)V->getNext();
//        }
//        //return out;
//        outs[0] = left;
//        outs[1] = right;
//      }
//
//    //----------
//
//    void postProcess(void)
//      {
//        axVoice* V = (axVoice*)mReleasedVoices.getHead();
//        while (V)
//        {
//          if (V->mState==vs_Off)
//          {
//            axVoice* next = (axVoice*)V->getNext();
//            mReleasedVoices.remove(V);
//            mFreeVoices.append(V);
//            V = next;
//          }
//          else V = (axVoice*)V->getNext();
//        }
//        mNumEvents = 0;
//      }
//
//};
//
////----------------------------------------------------------------------
//#endif
