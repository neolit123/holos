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
#ifndef h_Host_Vst_impl_included
#define h_Host_Vst_impl_included
//#ifdef h_Vst_included
//----------------------------------------------------------------------

// TODO:
// move all methods from instance that uses audioMaster to host

h_Host::h_Host(audioMasterCallback audioMaster, AEffect* a_AEffect)
: h_Host_Base()
  {
    m_AudioMaster = audioMaster;
    m_AEffect = a_AEffect;
    //NOTE: AEffect has not been prepared yet..
  }

//----------

h_Host::~h_Host()
  {
  }

//----------

h_String h_Host::getName(void)
  {
    return "vst";
  }

//----------------------------------------------------------------------
//
// public
//
//----------------------------------------------------------------------

//TODO

//----------------------------------------------------------------------
//
// vst
//
//----------------------------------------------------------------------

// returns the host's version (for example 2400 for VST 2.4)
// [return value]: Host VST version (for example 2400 for VST 2.4)
// @see AudioEffect::getMasterVersion

VstInt32 h_Host::vst_Version(void)
  {
    VstInt32 version = 1;
    if (m_AudioMaster) version = (VstInt32)m_AudioMaster(m_AEffect,audioMasterVersion,0,0,0,0);
    if (!version)	version = 1; // old
    return version;
  }

//----------

// returns current unique identifier when loading shell plug-ins
// [return value]: current unique identifier on shell plug-in
// @see AudioEffect::getCurrentUniqueId

VstInt32 h_Host::vst_CurrentId()
  {
    VstInt32 id = 0;
    if (m_AudioMaster) id = (VstInt32)m_AudioMaster(m_AEffect,audioMasterCurrentId,0,0,0,0);
    return id;
  }

//----------

// called after a control has changed in the editor and when the associated parameter should be automated
// [index]: parameter index
// [opt]: parameter value
// @see AudioEffect::setParameterAutomated
//
// An important thing to notice is that if the user changes a parameter in your editor, which is
// out of the Host's control if you are not using the default string based interface, you should
// call setParameterAutomated (). This ensures that the Host is notified of the parameter change, which
// allows it to record these changes for automation.

void h_Host::vst_Automate(VstInt32 index, float value)
  {
    if (m_AudioMaster) m_AudioMaster(m_AEffect,audioMasterAutomate,index,0,0,value);
    //setParameter(index,value);
  }

//----------

// Give idle time to Host application,
// e.g. if plug-in editor is doing mouse tracking in a modal loop.
// no arguments
// @see AudioEffect::masterIdle
// masterIdle

void h_Host::vst_Idle(void)
  {
    if (m_AudioMaster) m_AudioMaster(m_AEffect,audioMasterIdle,0,0,0,0);
  }

//----------

//enum VstTimeInfoFlags
//{
//	kVstTransportChanged     = 1,		///< indicates that play, cycle or record state has changed
//	kVstTransportPlaying     = 1 << 1,	///< set if Host sequencer is currently playing
//	kVstTransportCycleActive = 1 << 2,	///< set if Host sequencer is in cycle mode
//	kVstTransportRecording   = 1 << 3,	///< set if Host sequencer is in record mode
//	kVstAutomationWriting    = 1 << 6,	///< set if automation write mode active (record parameter changes)
//	kVstAutomationReading    = 1 << 7,	///< set if automation read mode active (play parameter changes)

//	kVstNanosValid           = 1 << 8,	///< VstTimeInfo::nanoSeconds valid
//	kVstPpqPosValid          = 1 << 9,	///< VstTimeInfo::ppqPos valid
//	kVstTempoValid           = 1 << 10,	///< VstTimeInfo::tempo valid
//	kVstBarsValid            = 1 << 11,	///< VstTimeInfo::barStartPos valid
//	kVstCyclePosValid        = 1 << 12,	///< VstTimeInfo::cycleStartPos and VstTimeInfo::cycleEndPos valid
//	kVstTimeSigValid         = 1 << 13,	///< VstTimeInfo::timeSigNumerator and VstTimeInfo::timeSigDenominator valid
//	kVstSmpteValid           = 1 << 14,	///< VstTimeInfo::smpteOffset and VstTimeInfo::smpteFrameRate valid
//	kVstClockValid           = 1 << 15	///< VstTimeInfo::samplesToNextClock valid
//};

//struct VstTimeInfo
//{
//	double samplePos;				///< current Position in audio samples (always valid)
//	double sampleRate;				///< current Sample Rate in Herz (always valid)
//	double nanoSeconds;				///< System Time in nanoseconds (10^-9 second)
//	double ppqPos;					///< Musical Position, in Quarter Note (1.0 equals 1 Quarter Note)
//	double tempo;					///< current Tempo in BPM (Beats Per Minute)
//	double barStartPos;				///< last Bar Start Position, in Quarter Note
//	double cycleStartPos;			///< Cycle Start (left locator), in Quarter Note
//	double cycleEndPos;				///< Cycle End (right locator), in Quarter Note
//	VstInt32 timeSigNumerator;		///< Time Signature Numerator (e.g. 3 for 3/4)
//	VstInt32 timeSigDenominator;	///< Time Signature Denominator (e.g. 4 for 3/4)
//	VstInt32 smpteOffset;			///< SMPTE offset (in SMPTE subframes (bits; 1/80 of a frame)). The current SMPTE position can be calculated using #samplePos, #sampleRate, and #smpteFrameRate.
//	VstInt32 smpteFrameRate;		///< @see VstSmpteFrameRate
//	VstInt32 samplesToNextClock;	///< MIDI Clock Resolution (24 Per Quarter Note), can be negative (nearest clock)
//	VstInt32 flags;					///< @see VstTimeInfoFlags
//};

// audioMasterGetTime,
// [value]: request mask
// [return value]: #VstTimeInfo* or null if not supported
// @see VstTimeInfoFlags @see AudioEffectX::getTimeInfo

VstTimeInfo* h_Host::vst_GetTime(VstInt32 filter)
  {
    if (m_AudioMaster)
    {
      VstIntPtr ret = m_AudioMaster(m_AEffect,audioMasterGetTime,0,filter,0,0);
      return FromVstPtr<VstTimeInfo> (ret);
    }
    return 0;
  }

//----------

//audioMasterProcessEvents,
// [ptr]: pointer to #VstEvents  @see VstEvents @see AudioEffectX::sendVstEventsToHost
// Can be called inside processReplacing.
// param events Fill with VST events
// return Returns \e true on success

bool h_Host::vst_ProcessEvents(VstEvents* events)
  {
    if (m_AudioMaster) return m_AudioMaster(m_AEffect,audioMasterProcessEvents,0,0,events,0)==1;
    return false;
  }

//----------

//audioMasterIOChanged,
// [return value]: 1 if supported
// @see AudioEffectX::ioChanged
//
// The Host could call a suspend() (if the plug-in was enabled (in resume() state)) and then ask for
// getSpeakerArrangement() and/or check the \e numInputs and \e numOutputs and \e initialDelay and then call a
// resume().
// return true on success
// see also: setSpeakerArrangement(), getSpeakerArrangement()

bool h_Host::vst_IOChanged(void)
  {
    if (m_AudioMaster) return (m_AudioMaster(m_AEffect,audioMasterIOChanged,0,0,0,0) != 0);
    return false;
  }

//----------

//audioMasterSizeWindow,
// [index]: new width
// [value]: new height
// [return value]: 1 if supported  @see AudioEffectX::sizeWindow

bool h_Host::vst_SizeWindow(int aWidth, int aHeight)
  {
    if (m_AudioMaster) return m_AudioMaster(m_AEffect,audioMasterSizeWindow,aWidth,aHeight,0,0)==1;
    return false;
  }

//----------

//audioMasterGetSampleRate,
// [return value]: current sample rate
// @see AudioEffectX::updateSampleRate

float h_Host::vst_GetSampleRate(void)
  {
    float rate = 0;
    if (m_AudioMaster)
    {
      VstIntPtr res = m_AudioMaster(m_AEffect, audioMasterGetSampleRate, 0, 0, 0, 0);
      if (res>0) rate = (float)res;
    }
    return rate;
  }

//----------

//audioMasterGetBlockSize,
// [return value]: current block size
// @see AudioEffectX::updateBlockSize

//audioMasterGetInputLatency,
// [return value]: input latency in audio samples
// @see AudioEffectX::getInputLatency

//audioMasterGetOutputLatency,
// [return value]: output latency in audio samples
// @see AudioEffectX::getOutputLatency

//audioMasterGetCurrentProcessLevel,
// [return value]: current process level
// @see VstProcessLevels

//audioMasterGetAutomationState,
// [return value]: current automation state
// @see VstAutomationStates

//audioMasterOfflineStart,
// [index]: numNewAudioFiles [value]: numAudioFiles [ptr]: #VstAudioFile*
// @see AudioEffectX::offlineStart

//audioMasterOfflineRead,
// [index]: bool readSource [value]: #VstOfflineOption*
// @see VstOfflineOption [ptr]: #VstOfflineTask*
// @see VstOfflineTask
// @see AudioEffectX::offlineRead

//audioMasterOfflineWrite,
// @see audioMasterOfflineRead
// @see AudioEffectX::offlineRead

//audioMasterOfflineGetCurrentPass,
// @see AudioEffectX::offlineGetCurrentPass

//audioMasterOfflineGetCurrentMetaPass,
// @see AudioEffectX::offlineGetCurrentMetaPass

//audioMasterGetVendorString,
// [ptr]: char buffer for vendor string, limited to #kVstMaxVendorStrLen
// @see AudioEffectX::getHostVendorString

//----------

bool h_Host::vst_GetVendorString(char* buffer)
  {
    if (m_AudioMaster) return m_AudioMaster(m_AEffect,audioMasterGetVendorString,0,0,buffer,0)==1;
    return false;
  }

//----------

//audioMasterGetProductString,
// [ptr]: char buffer for vendor string, limited to #kVstMaxProductStrLen
// @see AudioEffectX::getHostProductString

bool h_Host::vst_GetProductString(char* buffer)
  {
    if (m_AudioMaster) return m_AudioMaster(m_AEffect,audioMasterGetProductString,0,0,buffer,0)==1;
    return false;
  }

//----------

//audioMasterGetVendorVersion,
// [return value]: vendor-specific version
// @see AudioEffectX::getHostVendorVersion

int h_Host::vst_GetVendorVersion(void)
  {
    if (m_AudioMaster) return m_AudioMaster(m_AEffect,audioMasterGetVendorVersion,0,0,0,0)==1;
    return 0;
  }

//----------

//audioMasterVendorSpecific,
// no definition, vendor specific handling
// @see AudioEffectX::hostVendorSpecific

//----------

//namespace HostCanDos
//{
//	const char* canDoSendVstEvents = "sendVstEvents"; ///< Host supports send of Vst events to plug-in
//	const char* canDoSendVstMidiEvent = "sendVstMidiEvent"; ///< Host supports send of MIDI events to plug-in
//	const char* canDoSendVstTimeInfo = "sendVstTimeInfo"; ///< Host supports send of VstTimeInfo to plug-in
//	const char* canDoReceiveVstEvents = "receiveVstEvents"; ///< Host can receive Vst events from plug-in
//	const char* canDoReceiveVstMidiEvent = "receiveVstMidiEvent"; ///< Host can receive MIDI events from plug-in
//	const char* canDoReportConnectionChanges = "reportConnectionChanges"; ///< Host will indicates the plug-in when something change in plug-in´s routing/connections with #suspend/#resume/#setSpeakerArrangement
//	const char* canDoAcceptIOChanges = "acceptIOChanges"; ///< Host supports #ioChanged ()
//	const char* canDoSizeWindow = "sizeWindow"; ///< used by VSTGUI
//	const char* canDoOffline = "offline"; ///< Host supports offline feature
//	const char* canDoOpenFileSelector = "openFileSelector"; ///< Host supports function #openFileSelector ()
//	const char* canDoCloseFileSelector = "closeFileSelector"; ///< Host supports function #closeFileSelector ()
//	const char* canDoStartStopProcess = "startStopProcess"; ///< Host supports functions #startProcess () and #stopProcess ()
//	const char* canDoShellCategory = "shellCategory"; ///< 'shell' handling via uniqueID. If supported by the Host and the Plug-in has the category #kPlugCategShell
//	const char* canDoSendVstMidiEventFlagIsRealtime = "sendVstMidiEventFlagIsRealtime"; ///< Host supports flags for #VstMidiEvent
//}

//audioMasterCanDo,
// [ptr]: "can do" string
// [return value]: 1 for supported

bool h_Host::vst_CanDo(const char* str)
  {
    if (m_AudioMaster) return (m_AudioMaster(m_AEffect,audioMasterCanDo,0,0,(void*)str,0)==1);
    return false;
  }

//audioMasterGetLanguage,
// [return value]: language code
// @see VstHostLanguage

//audioMasterGetDirectory,
// [return value]: FSSpec on MAC, else char*
// @see AudioEffectX::getDirectory

//audioMasterUpdateDisplay,
// no arguments

//----------

//audioMasterBeginEdit,
// [index]: parameter index
// @see AudioEffectX::beginEdit
//
// tells the Host that if it needs to, it has to record automation data for this control.
// param index Index of the parameter
// Returns true on success

bool h_Host::vst_BeginEdit(VstInt32 index)
  {
    if (m_AudioMaster) return (m_AudioMaster(m_AEffect,audioMasterBeginEdit,index,0,0,0)) ? true : false;
    return 0;
  }

//----------

//audioMasterEndEdit,
// [index]: parameter index
// @see AudioEffectX::endEdit
//
// notifies the Host that this control is no longer moved by the mouse.
// param index Index of the parameter
// Returns \e true on success

bool h_Host::vst_EndEdit(VstInt32 index)
{
  if (m_AudioMaster) return (m_AudioMaster(m_AEffect,audioMasterEndEdit,index,0,0,0)) ? true : false;
  return 0;
}

//----------

//audioMasterOpenFileSelector,
// [ptr]: VstFileSelect*
// [return value]: 1 if supported
// @see AudioEffectX::openFileSelector

//audioMasterCloseFileSelector,
// [ptr]: VstFileSelect*
// @see AudioEffectX::closeFileSelector

//----------------------------------------------------------------------
//#endif
#endif
