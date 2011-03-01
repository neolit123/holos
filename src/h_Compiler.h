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
#ifndef h_Compiler_included
#define h_Compiler_included
//----------------------------------------------------------------------

/* TODO:
    - 2-pass, 2nd to resolve adresses to labels and words
    - macro, insert opcodes directly, instead of func call
    - do some of this in parsing...
    - jit (?)
*/

#include "src/h_Stack.h"
#include "src/h_String.h"
#include "src/h_VirtualMachine.h"

#define VM_NUM_OPCODES    65536
#define VM_MAX_CONDSTACK  32

//----------------------------------------------------------------------

struct h_ScrWord
{
  char    name[16];
  int     pos;
};

struct h_ScrLabel
{
  char    name[16];
  int     pos;
};

struct h_ScrVar
{
  char    name[16];
  int     pos;
};

typedef h_Array<h_ScrWord>  h_ScrWords;
typedef h_Array<h_ScrLabel> h_ScrLabels;
typedef h_Array<h_ScrVar>   h_ScrVars;
typedef h_Stack<int,VM_MAX_CONDSTACK> h_CondStack;

//----------------------------------------------------------------------

class h_Compiler
{
  private:
    char*       m_Sourcecode;
    int         m_Sourcecode_size;
    h_Opcode*   m_Opcodes;
    int         m_NumOpcodes;
    h_Strings   m_Tokens;
    h_ScrWords  m_Words;
    h_ScrLabels m_Labels;
    h_ScrVars   m_Vars;
    h_CondStack m_CondStack;
  private:
    int   m_Pos;
    bool  m_NewWord;
    bool  m_NewLabel;
    bool  m_NewVar;

  public:

    h_Compiler()
      {
        m_Sourcecode = NULL;
        m_Sourcecode_size = 0;
        m_Opcodes = new h_Opcode[VM_NUM_OPCODES];
        m_NumOpcodes = 0;
        m_Pos = 0;
        m_NewWord = false;
        m_NewLabel = false;
        m_NewVar = false;
      }

    //----------

    ~h_Compiler()
      {
        if (m_Sourcecode) delete[] m_Sourcecode;
        if (m_Opcodes) delete[] m_Opcodes;
        // delete words
        // delete labels
      }

    //----------------------------------------

    inline int getNumOpcodes(void) { return m_NumOpcodes; }
    inline h_Opcode* getOpcodes(void) { return m_Opcodes; }

    //----------------------------------------

    // virtual
    int findWord(char* token)
      {
        for (int i=0; i<m_Words.size(); i++) { if (h_Strcmp(m_Words[i].name,token)==0) return m_Words[i].pos; }
        return -1;
      }

    //----------

    // virtual
    int findLabel(char* token)
      {
        for (int i=0; i<m_Labels.size(); i++) { if (h_Strcmp(m_Labels[i].name,token)==0) return m_Labels[i].pos; }
        return -1;
      }

    //----------

    // virtual
    int findVar(char* token)
      {
        for (int i=0; i<m_Vars.size(); i++) { if (h_Strcmp(m_Vars[i].name,token)==0) return m_Vars[i].pos; }
        return -1;
      }

//    //----------------------------------------
//    //
//    //----------------------------------------
//
//    int _find_internal(char* token)
//      {
//        for (int i=0; i<VM_NUM_INTERNALS; i++) { if (h_Strcmp(h_InternalNames[i],token)==0) return i; }
//        return -1;
//      }
//
//    //----------
//
//    int _find_word(char* token)
//      {
//        for (int i=0; i<m_Words.size(); i++) { if (h_Strcmp(m_Words[i].name,token)==0) return i; }
//        return -1;
//      }
//
//    //----------
//
//    int _find_label(char* token)
//      {
//        for (int i=0; i<m_Labels.size(); i++) { if (h_Strcmp(m_Labels[i].name,token)==0) return i; }
//        return -1;
//      }
//
    //----------------------------------------
    //
    // eparse
    //
    //----------------------------------------

    // char* -> m_Tokens

    // virtual
    void parse(char* a_Source, int a_Size)
      {
        m_Sourcecode_size = a_Size;
        m_Sourcecode = new char[a_Size+1];
        h_Memcpy(m_Sourcecode,a_Source,a_Size);
        m_Sourcecode[a_Size] = 0;
        int pos = 0;
        char prev = 0;
        while (pos<a_Size)
        {
          unsigned char ch = m_Sourcecode[pos];
          if (ch<=32) { ch=0; m_Sourcecode[pos]=0; }
          else if (prev==0) m_Tokens.append( h_String(&m_Sourcecode[pos]) );
          prev = ch;
          pos++;
        }
        // todo: parse m_Tokens into new list, woth macros, preprocessor..
      }

    //----------------------------------------

    // m_Tokens -> m_Opcodes

    #define WRITE_op(t,v) m_Opcodes[m_Pos].type=t; \
                          m_Opcodes[m_Pos].value=(h_Cell)v; \
                          m_Pos++

    #define PUSH_cond     m_CondStack.push(m_Pos)
    #define POP_cond      m_CondStack.pop()

    //----------------------------------------

    int _find_internal(char* token)
      {
        for (int i=0; i<VM_NUM_INTERNALS; i++) { if (h_Strcmp(h_InternalNames[i],token)==0) return i; }
        return -1;
      }

    //----------

    int _find_word(char* token)
      {
        for (int i=0; i<m_Words.size(); i++) { if (h_Strcmp(m_Words[i].name,token)==0) return i; }
        return -1;
      }

    //----------

    int _find_label(char* token)
      {
        for (int i=0; i<m_Labels.size(); i++) { if (h_Strcmp(m_Labels[i].name,token)==0) return i; }
        return -1;
      }

    //----------

    int _find_var(char* token)
      {
        for (int i=0; i<m_Vars.size(); i++) { if (h_Strcmp(m_Vars[i].name,token)==0) return i; }
        return -1;
      }

    //----------------------------------------
    //
    // encode
    //
    //----------------------------------------

    // ( call, ) = return
    bool encode_newword(char* token)
      {
        if ( (h_Strcmp(token,"(")==0) )
        {
          m_NewWord = true;
          return true;
        }
        else
        if (h_Strcmp(token,")")==0)
        {
          WRITE_op(ot_internal,si_ret);
          m_NewWord = false;
          return true;
        }
        else
        if (m_NewWord)
        {
          h_ScrWord word;
          word.pos = m_Pos;
          h_Strcpy(word.name,token);
          m_Words.append(word);
          m_NewWord = false;
          return true;
        }
        return false;
      }

    //----------

    // &
    bool encode_newlabel(char* token)
      {
        if ( (h_Strcmp(token,"&")==0) )
        {
          m_NewLabel = true;
          return true;
        }
        else
        if (m_NewLabel)
        {
          h_ScrLabel label;
          label.pos = m_Pos;
          h_Strcpy(label.name,token);
          m_Labels.append(label);
          m_NewLabel = false;
          return true;
        }
        return false;
      }

    //----------

    // %
    bool encode_newvar(char* token)
      {
        if ( (h_Strcmp(token,"%")==0) )
        {
          m_NewVar = true;
          return true;
        }
        else
        if (m_NewVar)
        {
          h_ScrVar var;
          var.pos = m_Vars.size();
          h_Strcpy(var.name,token);
          m_Vars.append(var);
          m_NewVar = false;
          return true;
        }
        return false;
      }

    //----------

    // ?:;
    bool encode_conditional(char* token)
      {
        if ( (h_Strcmp(token,"?")==0) ) // if
        {
          WRITE_op(ot_internal,si_jz);  PUSH_cond;
          WRITE_op(ot_codeptr,0); // temp
          return true;
        }
        else
        if ( (h_Strcmp(token,":")==0) ) // else
        {
          int pos = POP_cond;
          WRITE_op(ot_internal,si_jmp);  PUSH_cond;
          WRITE_op(ot_codeptr,0); // temp
          m_Opcodes[pos].value = (h_Cell)m_Pos;
          return true;
        }
        else
        if ( (h_Strcmp(token,";")==0) ) // endif
        {
          int pos = POP_cond;
          m_Opcodes[pos].value = (h_Cell)m_Pos;
          return true;
        }
        return false;
      }

    //----------

    bool encode_word(char* token)
      {
        int word = _find_word(token);
        if (word>=0)
        {
          WRITE_op(ot_internal,si_call);
          WRITE_op(ot_codeptr, m_Words[word].pos );
          return true;
        }
        return false;
      }

    //----------

    bool encode_label(char* token)
      {
        int label = _find_label(token);
        if (label>=0)
        {
          //WRITE_op(ot_internal,si_jmp);
          WRITE_op(ot_codeptr, m_Labels[label].pos );
          return true;
        }
        return false;
      }

    //----------

    bool encode_var(char* token)
      {
        //trace("encode_var");
        if ( (h_Strcmp(token,"->")==0) )
        {
          WRITE_op(ot_internal,si_popd);
          return true;
        }
        else
        if ( (h_Strcmp(token,"<-")==0) )
        {
          WRITE_op(ot_internal,si_pushd);
          return true;
        }
        //else
        int var = _find_var(token);
        if (var>=0)
        {
          //WRITE_op(ot_internal,si_pushd); // push
          WRITE_op(ot_dataptr, m_Vars[var].pos );
          return true;
        }
        return false;
      }

    //----------

    bool encode_internal(char* token)
      {
        int internal = _find_internal(token);
        if (internal>=0)
        {
          WRITE_op(ot_internal,internal);
          return true;
        }
        return false;
      }

    //----------

    bool encode_number(char* token)
      {
        if (h_Strchr(token,'.')!=NULL)
        { // float
          float f = atof(token);
          WRITE_op(ot_internal,si_push);
          WRITE_op(ot_float, *(h_Cell*)&f );  // warning: dereferencing type-punned pointer will break strict-aliasing rules
                                              // TODO: the _may_alias 'trick ?
          return true;
        }
        else
        { // int
          int i = atoi(token);
          WRITE_op(ot_internal,si_push);
          WRITE_op(ot_int, i );
          return true;
        }
        return false;
      }

    //----------


    //----------------------------------------
    //
    // compile
    //
    //----------------------------------------

    //#define NEXT_t  m_Tokens[tok++].ptr()

    // virtual
    void compile(void)
      {
        m_Pos = 0;
        m_NewWord = false;

        int tok = 0;
        while ( tok < m_Tokens.size() )
        {
          //char* token = NEXT_t;
          char* token = m_Tokens[tok++].ptr();

          // try them all, until one reports 'true'
          if (!encode_newword(token))     // ()
          if (!encode_newlabel(token))    // &
          if (!encode_newvar(token))      // %
          if (!encode_conditional(token)) // ?:;
          if (!encode_word(token))
          if (!encode_label(token))
          if (!encode_var(token))
          if (!encode_internal(token))
          encode_number(token);

        } // while tok

        WRITE_op(ot_internal,si_exit);
        m_NumOpcodes = m_Pos;

      }

    //----------

};

//----------------------------------------------------------------------
#endif
