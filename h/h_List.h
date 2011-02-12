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
#ifndef h_List_included
#define h_List_included
//----------------------------------------------------------------------
// TODO:
// - templates
// - multiple inheritance (ala widgetListener, etc?

struct h_ListNode
{
  h_ListNode* m_PrevNode;
  h_ListNode* m_NextNode;
  //
  //h_ListNode() { m_PrevNode=NULL; m_NextNode=NULL; }
  //virtual ~h_ListNode() {}
  // accessors
  inline void prev(h_ListNode* a_Node) { m_PrevNode=a_Node; }
  inline void next(h_ListNode* a_Node) { m_NextNode=a_Node; }
  inline h_ListNode* prev(void)        { return m_PrevNode; }
  inline h_ListNode* next(void)        { return m_NextNode; }
};

//----------------------------------------------------------------------

class h_List
{
  private:
    //int         m_ListSize;
    h_ListNode* m_ListHead;
    h_ListNode* m_ListTail;

  public:

    h_List()
      {
        m_ListHead = NULL;
        m_ListTail = NULL;
        //m_ListSize = 0;
      }

    virtual ~h_List()
      {
      }

    //----------

    // accessors
    inline void         head(h_ListNode* a_Node)  { m_ListHead=a_Node; }
    inline void         tail(h_ListNode* a_Node)  { m_ListTail=a_Node; }
    inline h_ListNode*  head(void)                { return m_ListHead; }
    inline h_ListNode*  tail(void)                { return m_ListTail; }
    //inline int          size(void)                { return m_ListSize; }
    //inline int          incSize(void)             { m_ListSize++; return m_ListSize; }
    //inline int          decSize(void)             { m_ListSize--; return m_ListSize; }

    //----------

    //void clearList(void)
    //  {
    //    m_ListHead = NULL;
    //    m_ListTail = NULL;
    //    //m_ListSize = 0;
    //  }

    //----------

    // delete all nodes...
    void deleteNodes(void)
      {
        h_ListNode* node = m_ListHead;
        h_ListNode* next;
        while (node)
        {
          next = node->next();
          delete node;
          node = next;
        }
        m_ListHead = NULL;
        m_ListTail = NULL;
      }

    //----------

    // append to tail...
    void appendNode(h_ListNode* a_Node)
      {
        if (m_ListTail)
        {
          // xx XX --
          a_Node->prev(m_ListTail);
          a_Node->next(NULL);
          m_ListTail->next(a_Node);
          m_ListTail = a_Node;
        }
        else
        {
          // -- XX --
          m_ListHead = a_Node;
          a_Node->prev(NULL);
          a_Node->next(NULL);
          m_ListTail = a_Node;
        }
      }

    //----------

    // insert a_NewNode after a_Node
    void insertNode(h_ListNode* a_Node, h_ListNode* a_NewNode)
      {
        h_ListNode* next = a_Node->next();
        h_ListNode* prev = a_Node->prev();
        if (prev)
        {
          if (next)
          {
            //  xx XX xx
            prev->next(a_NewNode);
            a_NewNode->prev(prev);
            a_NewNode->next(next);
            next->prev(a_NewNode);
          }
          else
          {
            // xx XX --
            prev->next(a_NewNode);
            a_NewNode->prev(prev);
            a_NewNode->next(NULL);
            m_ListTail = a_NewNode;
          }
        }
        else // !head
        {
          if (next)
          {
            // -- XX xx
            m_ListHead = a_NewNode;
            a_NewNode->prev(NULL);
            a_NewNode->next(next);
            next->prev(a_NewNode);
          }
          else
          {
            // -- XX --
            m_ListHead = a_NewNode;
            a_NewNode->prev(NULL);
            a_NewNode->next(NULL);
            m_ListTail = a_NewNode;
          }
        } // !next
      }

    //----------

    // remove node
    void removeNode(h_ListNode* a_Node)
      {
        h_ListNode* next = a_Node->next();
        h_ListNode* prev = a_Node->prev();
        if (prev)
        {
          if (next)
          {
            //  xx XX xx
            // prev, next
            prev->next(next);
            next->prev(prev);
          }
          else
          {
            // xx XX --
            // prev, NULL
            // tail
            prev->next(NULL);
            m_ListTail = prev;
          }
        }
        else // !head
        {
          if (next)
          {
            // -- XX xx
            // NULL,next
            // head
            m_ListHead = next;
            next->prev(NULL);
          }
          else
          {
            // -- XX --
            // NULL,NULL
            // single
            m_ListHead = NULL;
            m_ListTail = NULL;
          }
        } // !next
      }

};

//----------------------------------------------------------------------
#endif
