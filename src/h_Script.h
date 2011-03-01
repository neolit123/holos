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
#ifndef h_Script_included
#define h_Script_included
//----------------------------------------------------------------------

#include "src/h_VirtualMachine.h"
#include "src/h_Compiler.h"

//----------------------------------------------------------------------

// combines the compiler and virtual machine into a single entity

class h_Script
{
  private:
    h_Compiler*       m_Compiler;
    h_VirtualMachine* m_VirtualMachine;

  public:

    h_Script()
      {
        m_Compiler = new h_Compiler();
        m_VirtualMachine = new h_VirtualMachine();
      }

    ~h_Script()
      {
        delete m_Compiler;
        delete m_VirtualMachine;
      }

    inline int getLength(void) { return m_Compiler->getNumOpcodes(); }

    void compile(char* source, int size)
      {
        m_Compiler->parse(source,size);
        m_Compiler->compile();
        h_Opcode* op = m_Compiler->getOpcodes();
        int numop = m_Compiler->getNumOpcodes();
        m_VirtualMachine->translate(op,numop);
      }

    int find(const char* label)
      {
        return m_Compiler->findLabel((char*)label);
      }

    void execute(int pos)
      {
        m_VirtualMachine->execute(pos);
      }
};

//----------------------------------------------------------------------
#endif


//  h_Compiler* COMP = new h_Compiler();
//  COMP->parse(sourcecode,sizeof(sourcecode));
//  COMP->compile();
//
//  h_Opcode* op = COMP->getOpcodes();
//  int numop = COMP->getNumOpcodes();
//
//  dtrace("num opcodes: " << numop);
//  //for (int i=0; i<numop; i++) dtrace(i << " " << op[i].type << ":" << op[i].value );
//  dtrace("");
//
//  h_VirtualMachine* VM = new h_VirtualMachine();
//  VM->translate(op,numop);
//
//  int entry = COMP->findLabel((char*)"main");
//  VM->execute(entry);
//
//  delete VM;
//  delete COMP;
