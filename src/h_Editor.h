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
#ifndef h_Editor_included
#define h_Editor_included
//----------------------------------------------------------------------

#include "src/h_Window.h"

struct h_Connection
{
  h_Parameter*  m_Parameter;
  h_Widget*     m_Widget;
  h_Connection(h_Parameter* a_Parameter, h_Widget* a_Widget)
    {
      m_Parameter = a_Parameter;
      m_Widget    = a_Widget;
    }
};

typedef h_Array<h_Connection*> h_Connections;

//----------------------------------------------------------------------

/*
  if there are rapid automation of a parameter from the host, there would be a
  cpu hit for all the redrawing, that might even happen too fast to notice.
  we cache the widget that needs to be redrawn, and paint them all in
  one IdleEditor (vst), or from a timer.
  also, we check if the widget is already in the update list, and only
  keep the last update (no point drawing the in-betweens).
*/

struct h_WidgetUpdateInfo
{
  h_Widget* m_Widget;
  int       m_Mode;
  h_WidgetUpdateInfo(h_Widget* a_Widget, int a_Mode)
    {
      m_Widget = a_Widget;
      m_Mode = a_Mode;
    }
};

typedef h_Array<h_WidgetUpdateInfo> h_WidgetUpdates;

// a h_Queue might be safer for a single-reader/single-writer, thread-wise
// or even better, a h_RingBuffer ??

//#include "src/h_Queue.h"
//#define H_WIDGET_QUEUE_SIZE 256
//typedef h_Queue<h_WidgetUpdateInfo,H_WIDGET_QUEUE_SIZE> m_WidgetUpdates;

//----------------------------------------------------------------------

class h_Editor : public h_Window
{
  //private:
  protected:
    h_Instance*     m_Instance;
    h_Connections   m_Connections;
    h_WidgetUpdates m_WidgetUpdates;
    h_Rect          m_WidgetUpdateRect;


  public:

    h_Editor(h_Instance* a_Instance, h_Rect a_Rect, void* a_Parent)
    : h_Window(this,a_Rect,a_Parent)
      {
        m_Instance = a_Instance;
      }

    virtual ~h_Editor()
      {
        #ifndef H_NOAUTODELETE
          deleteConnections();
        #endif
      }

    //----------------------------------------

    #ifdef H_BUFWDGUPDATES

    void clearUpdates(void)
      {
        //mutex_dirty.lock();
        m_WidgetUpdates.clear(false);
        m_WidgetUpdateRect.set(0,0,0,0);
        //mutex_dirty.unlock();
      }

    void appendUpdate(h_Widget* a_Widget, int a_Mode)
      {
        //for( int i=0; i<m_WidgetUpdates.size(); i++ ) if( m_WidgetUpdates[i].m_Widget==a_Widget ) return;
        m_WidgetUpdates.append( h_WidgetUpdateInfo(a_Widget,a_Mode));
        m_WidgetUpdateRect.combine( a_Widget->getRect() );
      }

    //TODO: consider threads, idleeditor, widget tweaking (do_MouseDown) ...
    // be careful so this doesn't crash with idle or wm_paint/expose events

    void redrawUpdates(void)
      {
        beginPaint();
        h_Rect rect = m_WidgetUpdateRect;
        int num = m_WidgetUpdates.size();
        for( int i=0; i<num; i++ )
        {
          h_Widget* widget = m_WidgetUpdates[i].m_Widget;
          int mode = m_WidgetUpdates[i].m_Mode;
          widget->do_Paint( getPainter(), rect, mode );
        }
        clearUpdates();
        endPaint();
      }

    #endif // H_BUFWDGUPDATES

    //----------

    virtual void idle(void)
      {
        #ifdef H_BUFWDGUPDATES
          redrawUpdates();
        #endif
      }

    virtual void connect(h_Parameter* a_Parameter, h_Widget* a_Widget)
      {
        int conn = m_Connections.size();
        a_Widget->setConnect(conn);
        a_Widget->setParameter(a_Parameter);
        a_Widget->setInternal( a_Parameter->getInternal() );
        a_Parameter->setConnect(conn);
        m_Connections.append( new h_Connection(a_Parameter,a_Widget) );
      }

    virtual void deleteConnections(void)
      {
        for (int i=0; i<m_Connections.size(); i++) delete m_Connections[i];
      }

    /*
    called from h_Instance [vst only, at the moment, in vst_setParameter}
    after value in parameter has been updated, do_HandleParameter() has been
    called, and if editor is open..
    */

    virtual void notifyParameter(h_Parameter* a_Parameter)
      {
        int conn = a_Parameter->getConnect();
        if (conn>=0)
        {
          h_Widget* wdg = m_Connections[conn]->m_Widget;
          wdg->setInternal( a_Parameter->getInternal() );
          #ifdef H_BUFWDGUPDATES
            appendUpdate(wdg,0);
            //redrawUpdates(); //TODO: in idle (or timer)
          #else
            on_Redraw(wdg,0);
          #endif
        }
      }

    /*
    widget value has changed (from editor)
    if widget connected to a parameter:
    - update parameter value (from widget)
    - notify instance
    */

    virtual void on_Change(h_Widget* a_Widget)
      {
        int conn = a_Widget->getConnect();
        if (conn>=0)
        {
          h_Parameter* par = m_Connections[conn]->m_Parameter;
          par->setInternal( a_Widget->getInternal() );
          m_Instance->notifyParameter(par);
        }
      }

};

//----------------------------------------------------------------------
#endif
