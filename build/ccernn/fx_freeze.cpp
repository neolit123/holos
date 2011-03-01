#define H_NOGUI
#include "holos.h"
#include "src/h_Parameter.h"

#define MAX_SRATE     192000
#define MAX_SECONDS   1
#define MAX_BUFSIZE   (MAX_SECONDS*MAX_SRATE*2)

//----------------------------------------------------------------------

const char* str_freeze[] =
{
  (char*)"off",
  (char*)"on"
};

const char* str_loopmode[] =
{
  (char*)"wrap",
  (char*)"bidi"
};

class my_Descriptor : public h_Descriptor
{
  public:

    my_Descriptor()
    : h_Descriptor()
      {
        m_Name   = "fx_freeze";
        m_Author = "ccernn";
        appendParameter( new h_ParFloat("buffer","ms",PF_DEFAULT,1000,1,1000) );
        appendParameter( new h_ParFloat("size",  "",  PF_DEFAULT,1,   0,1) );
        appendParameter( new h_ParFloat("speed", "",  PF_DEFAULT,1,   0,2) );
        appendParameter( new h_ParFloat("start", "",  PF_DEFAULT,0,   0,1) );
        appendParameter( new h_ParText ("freeze","",  PF_DEFAULT,0,   2, str_freeze) );
        appendParameter( new h_ParText ("mode","",    PF_DEFAULT,1,   2, str_loopmode) );
        initPrograms(10,&m_Parameters);
      }
};

#define H_DESCRIPTOR my_Descriptor

//----------------------------------------------------------------------

class my_Instance : public h_Instance
{
  private:
    float*  BUFFER;
    int     index;
    float   pos;

    int     bufsize;
    float   size;
    float   speed;
    float   start;
    int     freeze;
    int     mode;

    float   srate;
    float   _bufsize;
    float   _size;
    float   _start;

  public:

    my_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host,a_Descriptor)
      {
        BUFFER = new float[MAX_BUFSIZE];
        index  = 0;
        pos    = 0;
      }

    virtual ~my_Instance()
      {
        delete[] BUFFER;
      }

    virtual void do_HandleState(int a_State)
      {
        switch(a_State)
        {
          case is_Open:
            transferParameters();
            break;
        }
      }

    virtual void do_HandleParameter(h_Parameter* a_Parameter)
      {
        //if (aParameter->getIndex()==0) m_Gain = aParameter->getValue();
        int  id = a_Parameter->getIndex();
        float f = a_Parameter->getValue();
        switch(id)
        {
          case 0: _bufsize = f*0.001; break;
          case 1: _size    = f;       break;
          case 2:  speed   = f;       break;
          case 3: _start   = f;       break;
          case 4:  freeze  = f;       break;
          case 5:  mode    = f;       break;
        }
      }

    virtual bool do_ProcessBlock(float** a_Inputs, float** a_Outputs, int a_Size)
      {
        float srate = getSampleRate();
        bufsize = h_Max(1,(_bufsize*srate));
        size    = h_Max(1,(_size*bufsize));
        start   = _start * bufsize;
        while (index>=bufsize) index -= bufsize;
        //while (index<0) index += bufsize;
        return false;
      }

    virtual void do_ProcessSample(float** a_Inputs, float** a_Outputs)
      {
        float spl0 = *a_Inputs[0];
        float spl1 = *a_Inputs[1];

        int p2;
        if (freeze==0)
        {
          p2 = index*2;
          BUFFER[p2  ] = spl0;
          BUFFER[p2+1] = spl1;
        }
        index += 1;
        if (index>=bufsize) index = 0;
        //out = BUFFER[pos];
        pos += speed;
        if( mode==0) // wraparound
        {
          if (pos>=size) pos -= size;
          if (pos<0) pos += size;
        }
        else if (mode==1) // bidi looping
        {
          if (pos>=size)
          {
            pos = size - (pos-size);
            speed = -speed;
          }
          else if (pos<0)
          {
            pos = -pos;
            speed = -speed;
          }
        }
        p2 = start + pos;
        /*if (p>=bufsize)*/ while (p2>=bufsize) p2-=bufsize;
        /*if (p<0)*/ while (p2<0) p2+=bufsize;
        p2 *= 2;
        spl0 = BUFFER[p2];
        spl1 = BUFFER[p2+1];

        *a_Outputs[0] = spl0;
        *a_Outputs[1] = spl1;
      }

};

#define H_INSTANCE my_Instance

//----------------------------------------------------------------------
#include "holos_impl.h"




