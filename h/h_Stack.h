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
#ifndef h_Stack_included
#define h_Stack_included
//----------------------------------------------------------------------

// templated stack with static size

template<class _T,int SIZE>
class h_Stack
{
  private:
    _T  m_Buffer[SIZE];
    int m_Index;

  public:
    h_Stack()
      {
        reset();
      }
    ~h_Stack()
      {
      }

    void reset(void)
      {
        m_Index = 0;
      }

    inline
    void push(_T a_Item)
      {
        m_Buffer[m_Index] = a_Item;
        m_Index++;
      }

    inline
    _T pop(void)
      {
        m_Index--;
        return m_Buffer[m_Index];
      }

    // a few helper functions for the forth-like scripting language
    // see: h_Script, h_Compiler, h_VirtualMachine

    inline
    void dup(void)
      {
        m_Buffer[m_Index] = m_Buffer[m_Index-1];
        m_Index++;
      }

    inline
    void dup2(void)
      {
        m_Buffer[m_Index] = m_Buffer[m_Index-2];
        m_Index++;
      }

    inline
    void drop(void)
      {
        m_Index--;
      }

    inline
    _T* getTop(void)
      {
        return &m_Buffer[m_Index-1];
      }

};

//----------------------------------------------------------------------
#endif
