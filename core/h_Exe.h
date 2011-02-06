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
#ifndef h_Exe_included
#define h_Exe_included
//----------------------------------------------------------------------

class h_Host : public h_Host_Base
{
  public:
    h_Host();
    ~h_Host();

};

//----------------------------------------------------------------------

class h_Instance : public h_Instance_Base
{
  private:
    h_Host*       m_Host; // we need to delete this in destructor
    h_Descriptor* m_Descriptor;
    h_Rect        m_EditorRect;
    bool          m_EditorIsOpen;

  public:
    h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor);
    //: h_Instance_Base(h_Descriptor* a_Descriptor);
    virtual ~h_Instance();
    //
    //void  appendParameter(h_Parameter* a_Parameter);
    //void  deleteParameters(void);
    void  initParameters(void);
    void  prepareParameters(void);
    //void  notifyParameter(h_Parameter* aParameter);
    //void  notifyResize(int aWidth, int aHeight);
    //void  updateTime(void);
    //void  sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3);
    //
    inline int    getPlayState(void)                    { return 0;/*m_PlayState;*/ }
    inline double getSamplePos(void)                    { return 0;/*m_SamplePos;*/ }
    inline double getSampleRate(void)                   { return 0;/*if (m_SampleRate==0) updateTime(); return m_SampleRate;*/ }
    inline double getBeatPos(void)                      { return 0;/*return m_BeatPos;*/ }
    inline double getTempo(void)                        { return 0;/*return m_Tempo;*/ }
    inline int    getCurrentProgram(void)               { return 0;/*return m_CurrentProgram;*/ }
    //
    inline h_Rect getEditorRect(void)                   { return m_EditorRect; }
};

//----------------------------------------------------------------------

class h_Format
{
  private:
    h_Platform* m_Platform;

  public:
    h_Format();
    ~h_Format();
    //h_Descriptor* getDescriptor(void);
    h_Descriptor* createDescriptor(void);
    h_Instance*   createInstance(h_Host* a_Host,h_Descriptor* a_Descriptor);
    int           entrypoint(void* a_Ptr);
    h_String      getName(void) { return "exe"; }
};

//----------------------------------------------------------------------
#endif
