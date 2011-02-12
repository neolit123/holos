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
#ifndef h_Instance_Exe_cpp_included
#define h_Instance_Exe_cpp_included
//#ifdef h_Exe_included
//----------------------------------------------------------------------

h_Instance::h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
//: h_Instance_Base(a_Descriptor)
  {
    m_Host = a_Host;
    m_Descriptor = a_Descriptor;
    m_Parameters = &a_Descriptor->m_Parameters;
    m_EditorRect = m_Descriptor->m_EditorRect;// h_Rect(0,0,320,240);
    m_EditorIsOpen = false;
  }

//----------

h_Instance::~h_Instance()
  {
  }

//----------

//void h_Instance::appendParameter(h_Parameter* a_Parameter)
//  {
//    int index = m_Parameters->size();
//    a_Parameter->setIndex(index);
//    m_Parameters->append(a_Parameter);
//  }
//
//void h_Instance::deleteParameters(void)
//  {
//    //for (int i=0; i<m_Parameters->size(); i++) { delete m_Parameters->item(i); };
//  }

void h_Instance::prepareParameters(void)
  {
    int num = m_Parameters->size();
    for (int i=0; i<num; i++)
    {
      h_Parameter* par = m_Parameters->item(i);//[i];
      do_HandleParameter(par);
    }
  }

void h_Instance::notifyParameter_fromEditor(h_Parameter* a_Parameter)
  {
    do_HandleParameter(a_Parameter);
    //h_Instance_Base::notifyParameter_fromEditor(a_Parameter);
    //int index = a_Parameter->getIndex();
    //float value = a_Parameter->getInternal();//getValue();
    //trace("h_Instance::notifyParameter_fromEditor(" << index << "," << value <<  ")");
  }

void h_Instance::notifyResize_fromEditor(int a_Width, int a_Height)
  {
    //h_Instance_Base::notifyResize_fromEditor(a_Width,a_Height);
    //m_EditorRect.w = aWidth;
    //m_EditorRect.h = aHeight;
    //host_SizeWindow(aWidth, aHeight); // vst
    //m_Host->vst_SizeWindow(a_Width,a_Height); // vst
  }

void h_Instance::updateTime(void)
  {
    m_PlayState  = 0;
    m_SamplePos  = 0;
    m_SampleRate = 44100;
    m_BeatPos    = 0;
    m_Tempo      = 120;
  }

void h_Instance::sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3)
  {
  }


//----------------------------------------------------------------------
//#endif
#endif
