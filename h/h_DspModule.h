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
#ifndef h_DspModule_included
#define h_DspModule_included
//----------------------------------------------------------------------
///*
//
//  - a graph has a list of modules, and is-a (via inheritance) a module itself
//  - modules have two lists of pins, inputs and outputs
//  - modules are connected to eachother via these pins
//  - inputs pins have pointers directly to the source-pins data
//  - output pins have pointers to internally generated data
//  - graph.connect() connects two such pins, from output to input,
//  - initially, the modules are processed in the order they are appended to the graph
//  - compile() genertes a new module list, sorted by 'execution order'
//  - execute() executes this list (or the initial, non-sorted list if compile() has not been called
//  - a module can have additional functions, like the process() from 'the old' dspXXX classes
//  - a plugin has-a, or is-a graph, so that we just insert modules into the graph, and connect them to make a plugin
//    (can later be done visually with an editor that has "save as .cpp")
//    (or we can have this editor in a plugin)
//  - todo: script module
//  - todo: signal pins, going in the 'opposite direction', for control flow, events, etc.. calls doSignal()
//
//                   __________________           __________________
//                  |_____      _____  |         |_____      _____  |
//                  |     |    |     | |         |     |    |     | |
//                  | in  |    | out |<------------ in |    | out | |
//                  |_____|    |_____| |         |_____|    |_____| |
//                  |__________________|         |_____             |
//                                               |     |            |
//                                               |  in |            |
//                                               |_____|            |
//                                               |__________________|
//
//    (i'm obviously not a graphical artist! - ccernn)
//
//*/
////----------------------------------------------------------------------
//
//#include "core/axDefines.h"
//#include "core/axArray.h"
//
////----------------------------------------------------------------------
//
//// pin direction
//#define pd_Input  0
//#define pd_Output 1
//
//// pin type
//#define pt_None   0   // const
//#define pt_Signal 1   // static     Parameters, Midi, ..
//#define pt_Data   2   // dynamic    Audio
//
//// signal types
//#define st_None   0
//#define st_Gate   1
//
//// module flags
//#define mf_None   0
//#define mf_Active 1
//
////----------------------------------------------------------------------
//
//class axModule;
//
///*
//  a pin is essentially a ptr to a sample (and some helpers)
//  input pins: points directly to the value from/in the destination module/pin
//  output pins: points to the pin output value (internally in module)
//  these are manipulated in connect(), and doCompile(), etc...
//  to point directly (or as close as possibly) to the 'real data'
//*/
//
//class axPin
//{
//  protected:
//    axModule* mOwner;
//    axString  mName;
//    int       mType;
//    int       mDir;
//    SPL*      mPtr;
//  public:
//    //axPin() { mPinDir=pd_Output; mPinType=pt_Data; }
//    axPin(axModule *aOwner, axString aName, int aType, int aDir, SPL* aPtr=NULL)
//      {
//        mOwner = aOwner;
//        mName = aName;
//        mDir  = aDir;
//        mType = aType;
//        mPtr  = aPtr;
//      }
//    inline void       setPtr(SPL* aPtr) { mPtr=aPtr; }
//    inline SPL*       getPtr(void)      { return mPtr; }
//    inline int        getDir(void)      { return mDir; }
//    inline int        getType(void)     { return mType; }
//    inline axModule*  getOwner(void)    { return mOwner; }
//    inline axString   getName(void)     { return mName; }
//};
//
//typedef axArray<axPin*> axPins;
//
////----------------------------------------------------------------------
//
//// a listener is a way for the module to communicate 'upwards',
//// most probbaly a graph/macro, or our plugin
//// (if we want to 'intercept' the messages)
//
//class axModuleListener
//{
//  public:
//    virtual void onSignal(int aType=0, PTR aValue=NULL) {}
//};
//
////----------------------------------------------------------------------
//
///*
//  the default setting is a do-nothing in everything, so its quite useless
//  in a graph, but when you start filling in the virtual methods, it
//  gets more and more functionality
//*/
//
//class axModule
//{
//  protected:
//    axModuleListener* mListener;
//    axString  mName;
//    axPins    mPins;
//    int       mFlags;
//
//  public:
//
//    axModule(axModuleListener* aListener, axString aName)
//      {
//        mListener = aListener;
//        mName = aName;
//        mFlags = mf_Active;//mf_None;
//      }
//
//    virtual ~axModule()
//      {
//        #ifndef AX_NOAUTODELETE
//        deletePins();
//        #endif
//      }
//
//    // init
//
//    inline void     appendPin(axPin* aPin)  { mPins.append(aPin); }
//    inline void     deletePins(void)        { for (int i=0;i<mPins.size(); i++) delete mPins[i]; }
//    inline bool     isActive(void)          { return (mFlags&mf_Active); }
//
//    // access
//
//    virtual axPin*  getPin(int aIndex)                { return mPins[aIndex]; }
//    virtual SPL*    getPinPtr(int aIndex)             { return mPins[aIndex]->getPtr(); }
//    virtual void    setPinPtr(int aIndex, SPL* aPtr)  { mPins[aIndex]->setPtr(aPtr); }
//
//    // graph setup
//
//    //virtual int connectPin(axPin* mDstPin, axPin* aSrcPin, axModule* aSrcMod) { return 0; }
//    //virtual int connectDirect(axPin* mDstPin, SPL* aSource) { return 0; }
//    virtual int connectPin(int mDstPin, int aSrcPin, axModule* aSrcMod) { return 0; }
//    virtual int connectDirect(int mDstPin, SPL* aSource) { return 0; }
//
//    // runtime
//
//    // send signal to module
//    // events, execution flow opposite of audio.
//    // modules send events to their outputs
//    // for parameters, midi notes, etc,
//    // ex: midi module can send note event to oscillator, gate event to envelope, etc..
//    // also, for conditional execution
//    // (ex: multiple signal outputs, one of them selected depending on some input)
//    // aIndex = pin input index (which pin the signal is for)
//    // aType  = signal type
//    // aNum   = number (note, ctrl, etc..)
//    // aVal   = depending on number
//
//    //virtual void doSignal(int aType=0, PTR aValue=NULL) {}
//    virtual void doSignal(int aIndex, int aType, int aNum=0, float aVal=0) {}
//
//    virtual void doProcess(SPL** aInputs, SPL** aOutputs) {}
//
//    // prepares aExecList
//    // cache pointers, check connected inputs
//    // (todo: check pin rates, const, static dynamic)
//    // setup runtime execution state/mode.
//
//    virtual int doCompile(void) { return 0; }
//
//    // compiled (prepared) execution.
//    // use cached pointers, appropriate state/mode, etc..
//    // optimized code-path
//    //
//    // via getPinPtr(), we can get quite close to the source,
//    // so this kind of 'optimization' could be as effective as
//    // the normal process() funtions with pointer to the samples
//
//    //virtual /*int*/void doExecute(SPL** aInputs, SPL** aOutputs) { /*return 0;*/ }
//    virtual void doExecute(void) {}
//
//};
//
//typedef axArray<axModule*> axModules;
//
////----------------------------------------------------------------------
///*
//  what if we require that the modules cache everything needed,
//  so that the ptrs in the pins aren't used when calling the execlist?
//  that could make editing the graph at runtime safer? or more predictable?
//
//  can-do:
//    modScript : public axModule
//    opModule  : public axOpcode
//
//
//
//*/
//----------------------------------------------------------------------
#endif
