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
#ifndef h_Thread_included
#define h_Thread_included
//----------------------------------------------------------------------

//#include "lib/h_Defines.h"
#include "src/h_Defines.h"

//----------------------------------------------------------------------

class h_ThreadBase
{
  protected:
    bool  mThreadRunning;
    int   mThreadSleep;
  public:
    h_ThreadBase()                      { mThreadRunning=false; mThreadSleep=-1; }
    virtual ~h_ThreadBase()             {}
    virtual void startThread(int ms=-1) {} // -1 = no timer
    virtual void stopThread(void)       {}
    //
    // override this..
    // called at thread creation, or every timer tick if ms > 0
    virtual void doThreadFunc(void)     {}
    //
};

//----------------------------------------------------------------------
#ifdef H_LINUX
//----------------------------------------------------------------------

#include <pthread.h>
#include <unistd.h> // sleep

class h_Thread : public h_ThreadBase
{
  private:
    pthread_t mThreadHandle;
    //bool      mThreadRunning;
    //int       mThreadSleep;

  private:

    static void* threadProc(void* data)
      {
        h_Thread* thr = (h_Thread*)data;
        if (thr)
        {
          if (thr->mThreadSleep>=0)
          {
            while (thr->mThreadRunning)
            {
              thr->doThreadFunc();
              usleep(thr->mThreadSleep*1000); //ms*1000;
            }
          } else thr->doThreadFunc();
        }
        return NULL;
      }

  public:

    h_Thread()
    : h_ThreadBase()
      {
        mThreadHandle = 0;
        //mThreadRunning = false;
        //mThreadSleep = -1;
      }

    virtual ~h_Thread()
      {
        if (mThreadRunning) stopThread();
      }

    //virtual void doThreadFunc(void) {}

    virtual void startThread(int ms=-1)
      {
        mThreadSleep = ms;
        mThreadRunning = true;
        pthread_create(&mThreadHandle,NULL,&threadProc,this);
      }

    virtual void stopThread(void)
      {
        mThreadRunning = false;
        void* ret;
        pthread_join(mThreadHandle,&ret);
      }

};

#endif

//----------------------------------------------------------------------
#ifdef H_WIN32
//----------------------------------------------------------------------

#include <windows.h>
//#include <unistd.h> // sleep

class h_Thread : public h_ThreadBase
{
  private:
    HANDLE  mThreadHandle;
    DWORD   mThreadID;
      //bool  mThreadRunning;
      //int   mThreadSleep;

  private:

    static DWORD WINAPI threadProc(LPVOID data)
      {
        h_Thread* thr = (h_Thread*)data;
        if (thr)
        {
          if (thr->mThreadSleep>=0)
          {
            while (thr->mThreadRunning)
            {
              thr->doThreadFunc();
              Sleep(thr->mThreadSleep);
            }
          } else thr->doThreadFunc();
        }
        return H_NULL;
      }

  public:

    h_Thread() //: h_ThreadBase()
      {
        mThreadHandle = 0;
        mThreadID = 0;
        //mThreadRunning = false;
        //mThreadSleep = -1;
      }

    virtual ~h_Thread()
      {
        if (mThreadRunning) stopThread();
      }

    virtual void startThread(int ms=-1)
      {
        mThreadSleep   = ms;
        mThreadRunning = true;
        mThreadHandle = CreateThread(H_NULL,0,&threadProc,(LPVOID)this,H_NULL,&mThreadID);
      }

    virtual void stopThread(void)
      {
        //void* ret;
        mThreadRunning = false;
        //DWORD waiter = WaitForSingleObject(mThreadHandle,INFINITE);
        CloseHandle(mThreadHandle);
      }

};

#endif
//----------------------------------------------------------------------
#endif
