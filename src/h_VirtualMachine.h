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
#ifndef h_VirtualMachine_included
#define h_VirtualMachine_included
//----------------------------------------------------------------------

#include <stdio.h>  // printf

#include "src/h_Debug.h"
#include "src/h_Stack.h"

#define VM_CALL_STACK_SIZE    1024
#define VM_DATA_STACK_SIZE    1024
#define VM_CODE_SEGMENT_SIZE  65536
#define VM_DATA_SEGMENT_SIZE  65536

//----------------------------------------------------------------------

/*
  todo:
  integer type with size = void* for 32/64 bit
  fix all cell <-> int stuff below?
  speed is everything!

  am i right that we can't assume sizeof(int) == sizeof(void*)
  for both 32 and 64 bit ??
*/

typedef void* h_Cell;
//typedef long h_Cell; // void* (32bit)

typedef h_Stack<h_Cell, VM_DATA_STACK_SIZE> h_DataStack;
typedef h_Stack<h_Cell*,VM_CALL_STACK_SIZE> h_CallStack;

// opcode types
#define ot_none     0
#define ot_internal 1
#define ot_external 2
#define ot_word     3
#define ot_int      4
#define ot_float    5
#define ot_ptr      6
#define ot_dataptr  7
#define ot_codeptr  8

struct h_Opcode
{               // 32bit  64bit
  int type;     // 4      8?
  h_Cell value; // 4      8
};

//struct h_Bytecode
//{
//};

//----------------------------------------

enum h_ScriptInternals
{
  si_noop=0, si_exit,
  si_push,   si_drop, si_dup,  si_dup2,
  si_pushd,  si_popd,
  si_call,   si_ret,  si_jmp,  si_jz,   si_jnz,
  si_eq,     si_gr,   si_le,   si_ne,
  si_add,    si_sub,  si_mul,  si_div,
  si_inc,    si_dec,
  si_printi,
  VM_NUM_INTERNALS
};

const char* h_InternalNames[] =
{
  "noop","exit",
  "push","drop","dup", "dup2",
  "pushd","popd",
  "call","ret", "jmp", "jz",  "jnz",
  "=",   ">",   "<",   "!=",
  "+",   "-",   "*",   "/",
  "++",  "--",
  "."
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class h_VirtualMachine
{
  private:
    h_Opcode*   m_Opcodes;
    int         m_NumOpcodes;
    int         m_CodeSegment_size;
    h_Cell      m_CodeSegment[VM_CODE_SEGMENT_SIZE];
    h_Cell      m_DataSegment[VM_DATA_SEGMENT_SIZE];
    h_CallStack m_CallStack;
    h_DataStack m_DataStack;
    h_Cell*     m_IP;

  public:

    h_VirtualMachine()
      {
      }

    //----------

    ~h_VirtualMachine()
      {
      }

    //----------------------------------------

    // http://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html
    // You may not use this mechanism to jump to code in a different function.
    // If you do that, totally unpredictable things will happen.

    //virtual
    void translate(h_Opcode* a_Opcodes, int a_Size)
      {
        m_Opcodes = a_Opcodes;
        m_NumOpcodes = a_Size;
        execute(-1);
      }

    //----------

    //virtual
    void execute(int a_Position=0)
      {

        if (a_Position>=0)
        {

          //dtrace( "m_CodeSegment = " << (int)&m_CodeSegment[0] );
          //dtrace( "m_DataSegment = " << (int)&m_DataSegment[0] );

          //------------------------------
          // execute
          //------------------------------

          #define VM_NEXT       goto **m_IP++
          #define VM_NEXT_cell *m_IP++
          #define VM_PUSH_call  m_CallStack.push(m_IP)
          #define VM_POP_call   m_IP = m_CallStack.pop()
          #define VM_PUSH(x)    m_DataStack.push((h_Cell)x)
          #define VM_POP        m_DataStack.pop()
          #define VM_DROP       m_DataStack.drop()
          #define VM_DUP        m_DataStack.dup()
          #define VM_DUP2       m_DataStack.dup2()
          #define VM_POS(x)     m_CodeSegment[x]

          //----------

          m_IP = &m_CodeSegment[a_Position];
          VM_NEXT;

          // script internals

          _si_noop:
            VM_NEXT;

          _si_exit:
            return;

          _si_push:
            VM_PUSH(VM_NEXT_cell);
            VM_NEXT;

          _si_pushd: // push from data
            {
              h_Cell *dp = (h_Cell*)VM_NEXT_cell;
              VM_PUSH(*dp);
            }
            VM_NEXT;

          _si_popd: // pop to data
            {
              h_Cell *dp = (h_Cell*)VM_NEXT_cell;
              *dp = (h_Cell)VM_POP;
            }
            VM_NEXT;

          _si_drop:
            VM_DROP;
            VM_NEXT;

          _si_dup:
            VM_DUP;
            VM_NEXT;

          _si_dup2:
            VM_DUP2;
            VM_NEXT;

          //----------

          _si_call:
            {
              h_Cell *ip = (h_Cell*)VM_NEXT_cell;
              VM_PUSH_call;
              m_IP = ip;
            }
            VM_NEXT;

          _si_ret:
            VM_POP_call;
            VM_NEXT;

          _si_jmp:
            {
              h_Cell *ip = (h_Cell*)VM_NEXT_cell;
              m_IP = ip;
            }
            VM_NEXT;

          _si_jz:
            {
              h_Cell* ip = (h_Cell*)VM_NEXT_cell;
              if ( (int)VM_POP==0 ) m_IP = ip;
            }
            VM_NEXT;

          _si_jnz:
            {
              h_Cell* ip = (h_Cell*)VM_NEXT_cell;
              if ( (int)VM_POP!=0 ) m_IP = ip;
            }
            VM_NEXT;

          //----------

          _si_eq:
            {
              int v1 = (int)VM_POP;
              int v2 = (int)VM_POP;
              VM_PUSH((v2==v1));
            }
            VM_NEXT;

          _si_gr:
            {
              int v1 = (int)VM_POP;
              int v2 = (int)VM_POP;
              VM_PUSH((v2>v1));
            }
            VM_NEXT;

          _si_le:
            {
              int v1 = (int)VM_POP;
              int v2 = (int)VM_POP;
              VM_PUSH((v2<v1));
            }
            VM_NEXT;

          _si_ne:
            {
              int v1 = (int)VM_POP;
              int v2 = (int)VM_POP;
              VM_PUSH((v2!=v1));
            }
            VM_NEXT;

          //----------

          _si_add:
            {
              int v1 = (int)VM_POP;
              int v2 = (int)VM_POP;
              VM_PUSH((v2+v1));
            }
            VM_NEXT;

          _si_sub:
            {
              int v1 = (int)VM_POP;
              int v2 = (int)VM_POP;
              VM_PUSH((v2-v1));
            }
            VM_NEXT;

          _si_mul:
            {
              int v1 = (int)VM_POP;
              int v2 = (int)VM_POP;
              VM_PUSH((v2*v1));
            }
            VM_NEXT;

          _si_div:
            {
              int v1 = (int)VM_POP;
              int v2 = (int)VM_POP;
              VM_PUSH((v2/v1));
            }
            VM_NEXT;

          _si_inc:
            {
              //dtrace("inc");
              int* top = (int*)m_DataStack.getTop();
              //dtrace("*top: " << *top );
              *top+=1;
              //dtrace("*top: " << *top );
            }
            VM_NEXT;

          _si_dec:
            {
              int* top = (int*)m_DataStack.getTop();
              *top-=1;
            }
            VM_NEXT;

          //----------

          _si_printi:
            {
              int v = (int)VM_POP;
              printf("%i\n",v);
            }
            VM_NEXT;

          //------------------------------

        } // 0+ (exec)

        //else
        //if (a_Position==-2) // jit
        //{
        //} // -2 (jit)

        else
        if (a_Position==-1) // relocate
        {

          //------------------------------
          // translate
          //------------------------------

          // m_Opcodes -> m_CodeSegment ++ ...

          #define VM_WRITE(op) m_CodeSegment[pos++] = (h_Cell)op;

          void* op_adr[] =
          {
            &&_si_noop, &&_si_exit,
            &&_si_push, &&_si_drop, &&_si_dup,  &&_si_dup2,
            &&_si_pushd,&&_si_popd,
            &&_si_call, &&_si_ret,  &&_si_jmp,  &&_si_jz,   &&_si_jnz,
            &&_si_eq,   &&_si_gr,   &&_si_le,   &&_si_ne,
            &&_si_add,  &&_si_sub,  &&_si_mul,  &&_si_div,
            &&_si_inc,  &&_si_dec,
            &&_si_printi
          };

          int pos = 0;
          for (int i=0; i<m_NumOpcodes; i++)
          {
            int type = m_Opcodes[i].type;
            h_Cell value = m_Opcodes[i].value;
            switch (type)
            {

              //case ot_none:
              //  break;

              case ot_internal:
                VM_WRITE( op_adr[ (int)value ] );
                break;

              case ot_external:
              case ot_word:
              case ot_int:
              case ot_float:
              case ot_ptr:
                VM_WRITE(value);
                break;

              case ot_dataptr:
                VM_WRITE( &m_DataSegment[ (int)value ] );
                break;

              case ot_codeptr:
                VM_WRITE( &m_CodeSegment[ (int)value ] );
                break;

            } // switch type
          } // for size

          m_CodeSegment_size = pos;
        } // -1 (relocate)

      }

};

//----------------------------------------------------------------------
#endif
