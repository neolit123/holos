//#define H_DEBUG_LOG_HOME
//#define H_DEBUG_LOG "mverb.log"
//#define H_DEBUG_CON


//#define H_NOGUI
#define H_ALPHA

//#define H_DEBUG_MEM
//#define H_DEBUG_MEM_PRINT
//#define H_DEBUG_NEW

#include <math.h>     // sinf
#include <memory.h>   // memset
#include "holos.h"

#include "extern/mverb/MVerb.h"

#ifndef H_NOGUI
  #include "build/ports/port_mverb_skin.h"
  #include "src/h_WdgBackground.h"
  #include "src/h_WdgKnob.h"
#endif

//----------------------------------------------------------------------

class my_Descriptor : public h_Descriptor
{
  public:
    my_Descriptor()
    : h_Descriptor()
      {
        m_Name        = "mverb";
        m_Author      = "martin eastwood";
        m_Product     = "holos port";
        m_Version     = 2;
        m_UniqueId    = H_MAGIC + 0xf000;
        m_Flags       = df_None;
        m_NumInputs   = 2;
        m_NumOutputs  = 2;

        #ifndef H_NOGUI
        m_Flags |= df_HasEditor;
        m_EditorRect = h_Rect(456,108);
        #endif

        appendParameter( new h_Parameter("Damping",       "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Density",       "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Bandwidth",     "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Decay",         "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Predelay",      "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Size",          "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Gain",          "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Mix",           "", PF_DEFAULT, 0.5 ) );
        appendParameter( new h_Parameter("Early/Late Mix","", PF_DEFAULT, 0.5 ) );
      }
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

class my_Instance : public h_Instance
{
  private:
    MVerb<float> em_verb;
    #ifndef H_NOGUI
    h_Editor* m_Editor;
    port_mverb_skin* m_Skin;
    h_WdgKnob *k1,*k2,*k3,*k4,*k5,*k6,*k7,*k8,*k9;
    #endif

  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        /*
        char buffer[256];
        h_GetHomePath(buffer);
        trace( "h_GetHomePath: " << buffer );
        */
      }

    //----------

    virtual ~my_Instance()
      {
      }

    //----------

    virtual void do_HandleState(int a_State)
      {
        switch (a_State)
        {
          case is_Open:
            transferParameters(); // should we do this every time we suspend/resums?
            break;
          case is_Resume:
            em_verb.setSampleRate( getSampleRate());
            trace("samplerate: " << getSampleRate() );
            break;
        }
      }

    //----------

    virtual void do_HandleParameter(h_Parameter* a_Parameter)
      {
        int   index = a_Parameter->getIndex();
        float value = a_Parameter->getValue();
        trace("index: " << index << " = " << value);
        em_verb.setParameter(index,value);
      }

    //----------

    virtual bool do_ProcessBlock(float** a_Inputs, float** a_Outputs, int a_Size)
      {
        em_verb.process(a_Inputs,a_Outputs,a_Size);
        return true;
      }

    //--------------------------------------------------
    // gui
    //--------------------------------------------------

    #ifndef H_NOGUI

    virtual void* do_OpenEditor(void* ptr)
      {
        h_Rect rect = getEditorRect();
        m_Editor = new h_Editor(this,rect,ptr);
        m_Skin = new port_mverb_skin(m_Editor);
        m_Skin->load(m_Editor);
        m_Editor->applySkin(m_Skin);
        h_Widget* back = new h_WdgBackground(m_Editor);
        m_Editor->appendWidget( back );

        //back->appendWidget( k1 = new h_WdgKnob(m_Editor,h_Rect(0,0,32,32), 0.5) );

        back->appendWidget( k1 = new h_WdgKnob( m_Editor,h_Rect(336,40,40,64), 0.5 ) );
        back->appendWidget( k2 = new h_WdgKnob( m_Editor,h_Rect(216,40,40,64), 0.5 ) );
        back->appendWidget( k3 = new h_WdgKnob( m_Editor,h_Rect(256,40,40,64), 0.5 ) );
        back->appendWidget( k4 = new h_WdgKnob( m_Editor,h_Rect(296,40,40,64), 0.5 ) );
        back->appendWidget( k5 = new h_WdgKnob( m_Editor,h_Rect(96, 40,40,64), 0.5 ) );
        back->appendWidget( k6 = new h_WdgKnob( m_Editor,h_Rect(176,40,40,64), 0.5 ) );
        back->appendWidget( k7 = new h_WdgKnob( m_Editor,h_Rect(376,40,40,64), 0.5 ) );
        back->appendWidget( k8 = new h_WdgKnob( m_Editor,h_Rect(56, 40,40,64), 0.5 ) );
        back->appendWidget( k9 = new h_WdgKnob( m_Editor,h_Rect(136,40,40,64), 0.5 ) );

        // ugly!
        // automate in some way.. like a a for..loop

        k1->setFlag(wf_Alpha);
        k2->setFlag(wf_Alpha);
        k3->setFlag(wf_Alpha);
        k4->setFlag(wf_Alpha);
        k5->setFlag(wf_Alpha);
        k6->setFlag(wf_Alpha);
        k7->setFlag(wf_Alpha);
        k8->setFlag(wf_Alpha);
        k9->setFlag(wf_Alpha);

        m_Editor->connect( m_Parameters->item(0), k1 );
        m_Editor->connect( m_Parameters->item(1), k2 );
        m_Editor->connect( m_Parameters->item(2), k3 );
        m_Editor->connect( m_Parameters->item(3), k4 );
        m_Editor->connect( m_Parameters->item(4), k5 );
        m_Editor->connect( m_Parameters->item(5), k6 );
        m_Editor->connect( m_Parameters->item(6), k7 );
        m_Editor->connect( m_Parameters->item(7), k8 );
        m_Editor->connect( m_Parameters->item(8), k9 );

        m_Editor->do_SetSize(rect.w,rect.h); // realigns/paints buffer
        m_Editor->show();
        return (void*)m_Editor;
      }

    virtual void do_CloseEditor(void)
      {
        m_Editor->hide();
        delete m_Skin;
        delete m_Editor;
        m_Editor = H_NULL;
        // widgets are automatically deleted
      }

    //virtual void do_IdleEditor(void)
    //  {
    //  }

    #endif // H_NOGUI


};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"
