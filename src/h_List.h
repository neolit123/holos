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
//
// a templates (ala array)data-container)
// b multiple inheritance (ala widgetListener)

class h_DoubleListNode
{
  private:
    h_DoubleListNode* m_PrevListNode;
    h_DoubleListNode* m_NextListNode;
  public:
    inline void prev(h_DoubleListNode* a_Node) { m_PrevListNode=a_Node; }
    inline void next(h_DoubleListNode* a_Node) { m_NextListNode=a_Node; }
    inline h_DoubleListNode* prev(void) { return m_PrevListNode; }
    inline h_DoubleListNode* next(void) { return m_NextListNode; }
};

//----------

class h_DoubleList
{
  private:
    h_DoubleListNode* m_ListHead;
    h_DoubleListNode* m_ListTail;

  public:

    h_DoubleList()
      {
        m_ListHead = H_NULL;
        m_ListTail = H_NULL;
      }

    virtual ~h_DoubleList()
      {
      }

    inline void head(h_DoubleListNode* a_Node) { m_ListHead=a_Node; }
    inline void tail(h_DoubleListNode* a_Node) { m_ListTail=a_Node; }
    inline h_DoubleListNode* head(void) { return m_ListHead; }
    inline h_DoubleListNode* tail(void) { return m_ListTail; }

    // initialize the list
    virtual void clear(void)
      {
        m_ListHead = H_NULL;
        m_ListTail = H_NULL;
      }

    // delete all nodes...
    virtual void deleteNodes(void)
      {
        h_DoubleListNode* node = m_ListHead;
        h_DoubleListNode* next;
        while (node)
        {
          next = node->next();
          delete node;
          node = next;
        }
        m_ListHead = H_NULL;
        m_ListTail = H_NULL;
      }

    // append to tail...
    virtual void appendTail(h_DoubleListNode* a_Node)
      {
        if (m_ListTail)
        {
          // xx XX --
          a_Node->prev(m_ListTail);
          a_Node->next(H_NULL);
          m_ListTail->next(a_Node);
          m_ListTail = a_Node;
        }
        else
        {
          // -- XX --
          m_ListHead = a_Node;
          a_Node->prev(H_NULL);
          a_Node->next(H_NULL);
          m_ListTail = a_Node;
        }
      }

    // insert a_NewNode after a_Node
    virtual void insertNode(h_DoubleListNode* a_Node, h_DoubleListNode* a_NewNode)
      {
        h_DoubleListNode* next = a_Node->next();
        h_DoubleListNode* prev = a_Node->prev();
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
            a_NewNode->next(H_NULL);
            m_ListTail = a_NewNode;
          }
        }
        else // !head
        {
          if (next)
          {
            // -- XX xx
            m_ListHead = a_NewNode;
            a_NewNode->prev(H_NULL);
            a_NewNode->next(next);
            next->prev(a_NewNode);
          }
          else
          {
            // -- XX --
            m_ListHead = a_NewNode;
            a_NewNode->prev(H_NULL);
            a_NewNode->next(H_NULL);
            m_ListTail = a_NewNode;
          }
        } // !next
      }

    // remove node
    virtual void removeNode(h_DoubleListNode* a_Node)
      {
        h_DoubleListNode* next = a_Node->next();
        h_DoubleListNode* prev = a_Node->prev();
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
            prev->next(H_NULL);
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
            next->prev(H_NULL);
          }
          else
          {
            // -- XX --
            // NULL,NULL
            // single
            m_ListHead = H_NULL;
            m_ListTail = H_NULL;
          }
        } // !next
      }

};

//----------------------------------------------------------------------

typedef h_DoubleListNode  h_ListNode;
typedef h_DoubleList      h_List;

//----------------------------------------------------------------------
#endif
