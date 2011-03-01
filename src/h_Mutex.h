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
#ifndef h_Mutex_included
#define h_Mutex_included
//----------------------------------------------------------------------

//#include "lib/h_Defines.h"
#include "src/h_Defines.h"

//class h_MutexBase
//{
//  public:
//    h_MutexBase()               {}
//    virtual ~h_MutexBase()      {}
//    virtual void lock(void)     {}
//    virtual bool tryLock(void)  {return false;}
//    virtual void unlock(void)   {}
//};

//----------------------------------------------------------------------
#ifdef H_LINUX
//----------------------------------------------------------------------

// -lpthread
#include <pthread.h>

class h_Mutex//Impl : public h_MutexBase
{
  private:
    pthread_mutex_t mMutex;// = PTHREAD_MUTEX_INITIALIZER;
  public:
    h_Mutex/*Impl() : h_MutexBase*/() { pthread_mutex_init(&mMutex,NULL); }
    /*virtual*/ ~h_Mutex/*Impl*/()    { pthread_mutex_destroy(&mMutex); }
    /*virtual*/ void lock(void)       { pthread_mutex_lock(&mMutex); }
    /*virtual*/ bool tryLock(void)    { return (pthread_mutex_trylock(&mMutex)==0); }
    /*virtual*/ void unlock(void)     { pthread_mutex_unlock(&mMutex); }
};

#endif
//----------------------------------------------------------------------
#ifdef H_WIN32
//----------------------------------------------------------------------

#include <windows.h>

class h_Mutex//Impl : public h_MutexBase
{
  private:
    CRITICAL_SECTION  CriticalSection;
  public:
    h_Mutex/*Impl() : h_MutexBase*/() { InitializeCriticalSection(&CriticalSection); }
    /*virtual*/ ~h_Mutex/*Impl*/()    { DeleteCriticalSection(&CriticalSection); }
    /*virtual*/ void lock(void)       { EnterCriticalSection(&CriticalSection); }
    /*virtual*/ bool tryLock(void)    { return TryEnterCriticalSection( &CriticalSection ); }
    /*virtual*/ void unlock(void)     { LeaveCriticalSection(&CriticalSection); }
};

#endif
//----------------------------------------------------------------------

//typedef h_MutexImpl h_Mutex;

//----------------------------------------------------------------------
#endif
