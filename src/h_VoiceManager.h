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

// polyphonic voice manager


// - max block size: 999999 samples
//     a biiig offset that we will hopefully never reach
// - max events per block: #define MAX_EVENTS 1024
//     a static buffer for the incoming events, to avoid runtime memory juggling
// - very basic nna/note-stealing (works ok-ish)
// - currently only note on/off
// - converts events in processSample
//     could be done in processBlock (does it matter?)
// - not optimized
//     but 16 basic saw voices uses only around 0.8% cpu (reaper/wine)
//     so i've postponed that until it gets problematic
// - bugs?

//#include "lib/h_List.h"
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
//class h_Voice : public h_ListNode
//{
//  friend class h_VoiceManager;
//  protected:
//    int   mState;
//    float iRate;
//  public:
//    h_Voice() { mState = vs_Off; }
//    virtual ~h_Voice() {}
//    virtual void  setSampleRate(float aRate) { iRate = 1/aRate; }
//    virtual void  noteOn(int aNote, int aVel) {}
//    virtual void  noteOff(int aNote, int aVel) {}
//    virtual void  ctrlChange(int aCtrl, int aVal) {}
//    virtual void  control(int aIndex, float aVel) {}
//    //virtual float process(void) {return 0;}
//    virtual void  process(float* outs) {}
//};
//
//typedef h_Array<h_Voice*> h_Voices;
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
//class h_VoiceManager
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
//    h_Voice*  mNoteMap[128];
//    h_Voices  mAllVoices;
//    h_List    mFreeVoices;
//    h_List    mPlayingVoices;
//    h_List    mReleasedVoices;
//  public:
//
//    h_VoiceManager()
//      {
//        h_Memset(mNoteMap,0,sizeof(mNoteMap));
//        mOffset = 0;
//        mCurrEvent = 0;
//        mNextEvent = 999999;
//        mNumEvents = 0;
//        h_Memset(mEvents,0,sizeof(mEvents));
//        mNumPlaying = 0;
//      }
//
//    //----------
//
//    virtual ~h_VoiceManager()
//      {
//        #ifndef AX_NOAUTODELETE
//        deleteVoices();
//        #endif
//      }
//
//    //----------------------------------------
//
//    virtual void appendVoice(h_Voice* V)
//      {
//        mAllVoices.append(V);
//        mFreeVoices.appendNode(V);
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
//        h_Voice* V = (h_Voice*)mFreeVoices.tail();
//        if (V) { mFreeVoices.removeTail(); }
//        else
//        {
//          V = (h_Voice*)mReleasedVoices.getHead();
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
//        h_Voice* V = mNoteMap[aNote];
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
//        h_Voice* V = (h_Voice*)mPlayingVoices.getHead();
//        while (V)
//        {
//          //out += V->process();
//          V->process(&_outs[0]);
//          left += _outs[0];
//          right += _outs[1];
//          V = (h_Voice*)V->getNext();
//        }
//        // released voices
//        V = (h_Voice*)mReleasedVoices.getHead();
//        while (V)
//        {
//          if (V->mState!=vs_Off) //out += V->process();
//          {
//            V->process(&_outs[0]);
//            left += _outs[0];
//            right += _outs[1];
//          }
//          V = (h_Voice*)V->getNext();
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
//        h_Voice* V = (h_Voice*)mReleasedVoices.getHead();
//        while (V)
//        {
//          if (V->mState==vs_Off)
//          {
//            h_Voice* next = (h_Voice*)V->getNext();
//            mReleasedVoices.remove(V);
//            mFreeVoices.append(V);
//            V = next;
//          }
//          else V = (h_Voice*)V->getNext();
//        }
//        mNumEvents = 0;
//      }
//
//};

//----------------------------------------------------------------------
#endif
