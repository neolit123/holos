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
#ifndef h_AudioGraph_included
#define h_AudioGraph_included
//----------------------------------------------------------------------

///*
//  graph = collection of modules
//  called once per sample
//  your plugin (or a sample accurate event scheduler) takes care of
//  sample/event timing
//*/
////----------------------------------------------------------------------
//
//#include "lib/h_Defines.h"
//#include "audio/h_DspModule.h"
//
//class h_AudioGraph : public h_DspModule,
//                     public h_DspModuleListener
//{
//  protected:
//    h_DspModules mModules;
//    h_DspModules mExecList;
//    bool      mCompiled;
//  public:
//
//    h_AudioGraph(h_String aName)
//    : h_DspModule(NULL,aName)
//      {
//        mCompiled = false;
//      }
//
//    virtual ~h_AudioGraph()
//      {
//        #ifndef AX_NOAUTODELETE
//        deleteModules();
//        #endif
//      }
//
//    //----------
//
//    virtual void appendModule(h_DspModule* aModule) { mModules.append(aModule); }
//    virtual void deleteModules(void) { for (int i=0; i<mModules.size(); i++) delete mModules[i]; }
//
//    //----------------------------------------
//
////    //TODO: juggle input/output pointers
////    virtual int connectPin(axPin* mDstPin, axPin* aSrcPin, axModule* aSrcMod)
////      {
////        return 0;
////      }
//
////    //TODO: here too
////    virtual int connectDirect(axPin* mDstPin, SPL* aSource)
////      {
////        return 0;
////      }
//
////    virtual void doSignal(int aType=0, PTR aValue=NULL)
////      {
////        // parameters?
////        // midi?
////        // system messages? (start/stop/activate/..)
////      }
//
//    //----------------------------------------
//
//    // axModule[Base]
//
//    virtual void doProcess(SPL** aInputs, SPL** aOutputs)
//      {
//        for (int i=0; i<mModules.size(); i++)
//        {
//          //mModules[i]->doProcess(aInputs,aOutputs);
//          h_Module* moo = mModules[i];
//          if (moo->isActive()) moo->doProcess(aInputs,aOutputs);
//        }
//      }
//
//    //----------
//
//    // TODO:
//    //    make execlist based on which modules' output are needed first,
//    //    and the pin type/rate
//    // idea:
//    //    a primitive version of this is quite simple:
//    //    start with a mdule, append it onto the 'todo' queue/list,
//    //    then: repeatedly, until todo list is empty:
//    //    - get (and remove) module from todo queue
//    //    - append the modules that are connected to its input pins to the todo queue,
//    //    - append module to execlist (and remove from todo)
//    //      except if it's already in the list, if so, delete old one, append new (to the end)
//    //    then, postprocess the execlist by reversing it,
//    //    or by executing it 'in reverse'.
//
//    // naive compile: if module is active, put it into execlist
//    virtual int doCompile(void)
//      {
//        return 0;
//        mExecList.clear();
//        for (int i=0; i<mModules.size(); i++)
//        {
//          if (mModules[i]->isActive()) mExecList.append(mModules[i]);
//        }
//        mCompiled = true;
//      }
//
//    //----------
//
//
//    //virtual int doExecute(SPL** aInputs, SPL** aOutputs)
//    virtual /*int*/void doExecute(/*SPL** aInputs, SPL** aOutputs*/)
//      {
//        if (mCompiled)
//        {
//          for (int i=0; i<mExecList.size(); i++)
//          {
//            mExecList[i]->doExecute();//aInputs,aOutputs);
//          }
//          //return 0;
//        }
//        //else return -1;
//      }
//
//};
//

//----------------------------------------------------------------------
#endif
