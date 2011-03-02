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
#ifndef h_Format_Ladspa_impl_included
#define h_Format_Ladspa_impl_included
//#ifdef h_Ladspa_included
//----------------------------------------------------------------------

// static callback functions, host -> plugin
// do we need to have them static?

// called by host to instantiate a plugin..
// return the ptr to the instance (LADSPA_Handle)

//static
LADSPA_Handle lad_instantiate_callback(const LADSPA_Descriptor* Descriptor, unsigned long SampleRate)
  {
    h_Format* format = (h_Format*)Descriptor->ImplementationData;
    return format->lad_instantiate(Descriptor,SampleRate);
  }

// the rest are redirected to the instance(s)

//static
void lad_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
  {
    h_Instance* inst = (h_Instance*)Instance;
    inst->lad_connect_port(Port,DataLocation);
  }

//static
void lad_activate_callback(LADSPA_Handle Instance)
  {
    //axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
    //inst->lad_activate();
  }

//static
void lad_run_callback(LADSPA_Handle Instance, unsigned long SampleCount)
  {
    //axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
    //inst->lad_run(SampleCount);
  }

// we don't support this

//static
void lad_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount)
  {
    //axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
    //inst->lad_run_adding(SampleCount);
  }

// we don't support this

//static
void lad_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain)
  {
    //axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
    //inst->lad_set_run_adding_gain(Gain);
  }

//----------

//static
void lad_deactivate_callback(LADSPA_Handle Instance)
  {
    //axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
    //inst->lad_deactivate();
  }

//----------

static void lad_cleanup_callback(LADSPA_Handle Instance)
  {
    //axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
    //inst->lad_cleanup();
    //delete inst; // !!!
  }

//----------------------------------------------------------------------

h_Format::h_Format()
  {
  }

//----------

h_Format::~h_Format()
  {
  }

//----------

h_Descriptor* h_Format::createDescriptor(void)
  {
    return new H_DESCRIPTOR();
  }

//----------

h_Instance* h_Format::createInstance(h_Host* a_Host, h_Descriptor* a_Descriptor)
  {
    return new H_INSTANCE(a_Host,a_Descriptor);
  }

//----------

    // temporary
    // while setting up the port hints...

    /* Hint LADSPA_HINT_BOUNDED_BELOW indicates that the LowerBound field
       of the LADSPA_PortRangeHint should be considered meaningful. The
       value in this field should be considered the (inclusive) lower
       bound of the valid range. If LADSPA_HINT_SAMPLE_RATE is also
       specified then the value of LowerBound should be multiplied by the
       sample rate. */

    /* Hint LADSPA_HINT_BOUNDED_ABOVE indicates that the UpperBound field
       of the LADSPA_PortRangeHint should be considered meaningful. The
       value in this field should be considered the (inclusive) upper
       bound of the valid range. If LADSPA_HINT_SAMPLE_RATE is also
       specified then the value of UpperBound should be multiplied by the
       sample rate. */

    /* Hint LADSPA_HINT_TOGGLED indicates that the data item should be
       considered a Boolean toggle. Data less than or equal to zero should
       be considered `off' or `false,' and data above zero should be
       considered `on' or `true.' LADSPA_HINT_TOGGLED may not be used in
       conjunction with any other hint except LADSPA_HINT_DEFAULT_0 or
       LADSPA_HINT_DEFAULT_1. */

    /* Hint LADSPA_HINT_SAMPLE_RATE indicates that any bounds specified
       should be interpreted as multiples of the sample rate. For
       instance, a frequency range from 0Hz to the Nyquist frequency (half
       the sample rate) could be requested by this hint in conjunction
       with LowerBound = 0 and UpperBound = 0.5. Hosts that support bounds
       at all must support this hint to retain meaning. */

    /* Hint LADSPA_HINT_LOGARITHMIC indicates that it is likely that the
       user will find it more intuitive to view values using a logarithmic
       scale. This is particularly useful for frequencies and gains. */

    /* Hint LADSPA_HINT_INTEGER indicates that a user interface would
       probably wish to provide a stepped control taking only integer
       values. Any bounds set should be slightly wider than the actual
       integer range required to avoid floating point rounding errors. For
       instance, the integer set {0,1,2,3} might be described as [-0.1,
       3.1]. */

       //

    /* The various LADSPA_HINT_HAS_DEFAULT_* hints indicate a `normal'
       value for the port that is sensible as a default. For instance,
       this value is suitable for use as an initial value in a user
       interface or as a value the host might assign to a control port
       when the user has not provided one. Defaults are encoded using a
       mask so only one default may be specified for a port. Some of the
       hints make use of lower and upper bounds, in which case the
       relevant bound or bounds must be available and
       LADSPA_HINT_SAMPLE_RATE must be applied as usual. The resulting
       default must be rounded if LADSPA_HINT_INTEGER is present. Default
       values were introduced in LADSPA v1.1. */

    /* LADSPA_HINT_DEFAULT_NONE indicates that no default is provided. */

    /* LADSPA_HINT_DEFAULT_MINIMUM indicates that the suggested lower bound for the
       port should be used. */

    /* LADSPA_HINT_DEFAULT_LOW indicates that a low value between the suggested
       lower and upper bounds should be chosen. For ports with
       LADSPA_HINT_LOGARITHMIC, this should be exp(log(lower) * 0.75 +
       log(upper) * 0.25). Otherwise, this should be (lower * 0.75 + upper
       * 0.25). */

    /* LADSPA_HINT_DEFAULT_MIDDLE indicates that a middle value between the
       suggested lower and upper bounds should be chosen. For ports with
       LADSPA_HINT_LOGARITHMIC, this should be exp(log(lower) * 0.5 +
       log(upper) * 0.5). Otherwise, this should be (lower * 0.5 + upper *
       0.5). */

    /* LADSPA_HINT_DEFAULT_HIGH indicates that a high value between the suggested
       lower and upper bounds should be chosen. For ports with
       LADSPA_HINT_LOGARITHMIC, this should be exp(log(lower) * 0.25 +
       log(upper) * 0.75). Otherwise, this should be (lower * 0.25 + upper
       * 0.75). */

    /* LADSPA_HINT_DEFAULT_MAXIMUM indicates that the suggested upper bound for the
       port should be used. */

    /* LADSPA_HINT_DEFAULT_0 indicates that the number 0 should be used. Note
       that this default may be used in conjunction with
       LADSPA_HINT_TOGGLED. */

    /* LADSPA_HINT_DEFAULT_1 indicates that the number 1 should be used. Note
       that this default may be used in conjunction with
       LADSPA_HINT_TOGGLED. */

    /* LADSPA_HINT_DEFAULT_100 indicates that the number 100 should be used. */

    /* LADSPA_HINT_DEFAULT_440 indicates that the Hz frequency of `concert A'
       should be used. This will be 440 unless the host uses an unusual
       tuning convention, in which case it may be within a few Hz. */


    // some of these should probably be in the descriptor..
    /*
    char*                 mPortNames[MAX_LADSPA_PORTS];
    LADSPA_PortDescriptor mPortDesc[MAX_LADSPA_PORTS];
    LADSPA_PortRangeHint  mPortHint[MAX_LADSPA_PORTS];
    LADSPA_Descriptor     ladspadescr;
    */


LADSPA_Descriptor* h_Format::entrypoint(void* a_Ptr)
  {
    //trace("");

    m_Descriptor = createDescriptor();

    int i;
    int index = 0;
    // audio inputs
    for (i=0; i<m_Descriptor->m_NumInputs; i++)
    {
      m_PortNames[index]               = (char*)"input"; //m_Descriptor->getInputName(i);             // !!!
      m_PortDesc[index]                = LADSPA_PORT_AUDIO | LADSPA_PORT_INPUT;
      m_PortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_NONE;
      m_PortHint[index].LowerBound     = -1;
      m_PortHint[index].UpperBound     =  1;
      index++;
    }
    // audio outputs
    for (i=0; i<m_Descriptor->m_NumOutputs; i++)
    {
      m_PortNames[index]               = (char*)"output"; //mDescriptor->getOutputName(i);            // !!!
      m_PortDesc[index]                = LADSPA_PORT_AUDIO | LADSPA_PORT_OUTPUT;
      m_PortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_NONE;
      m_PortHint[index].LowerBound     = -1;
      m_PortHint[index].UpperBound     =  1;
      index++;
    }
    // parameters
    for (i=0; i<m_Descriptor->m_Parameters.size();  i++)
    {
      //axParamInfo paraminfo = mDescriptor->getParamInfo(i);
      m_PortNames[index] = m_Descriptor->m_Parameters[i]->getName().ptr(); //  paraminfo.mName;
      m_PortDesc[index]  = LADSPA_PORT_CONTROL | LADSPA_PORT_INPUT;
      LADSPA_PortRangeHintDescriptor hint = LADSPA_HINT_DEFAULT_NONE;
      /*
        TODO: we might need a way to get this info from a parameter (type, etc...)
        perhaps some function in parameter,
          getParamInfo(index,infostruct)
        that fills the infostruct with needed info
        a virtual function that each parameter type overrides...
      */

/*
      float val   = paraminfo.mDef - paraminfo.mMin;
           if (val == paraminfo.mMin ) mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_MINIMUM;
      else if (val == paraminfo.mMax ) mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_MAXIMUM;
      else if (val == 0 )   mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_0;
      else if (val == 1 )   mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_1;
      else if (val == 100 ) mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_100;
      else if (val == 440 ) mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_440;
      else
      {
        float sval  = (paraminfo.mDef-paraminfo.mMin) / (paraminfo.mMax-paraminfo.mMin);
        if (sval < 0.25) mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_LOW;
        else if (sval > 0.75) mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_HIGH;
        else mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_MIDDLE;
      }
      if (paraminfo.mType==pa_Pow) hint |= LADSPA_HINT_LOGARITHMIC;
      if (paraminfo.mType==pa_Int)
      {
        if (paraminfo.mMin==0 && paraminfo.mMax==1) hint |= LADSPA_HINT_TOGGLED;
        else hint |= LADSPA_HINT_INTEGER;
      }
*/

      m_PortHint[index].HintDescriptor = hint | LADSPA_HINT_BOUNDED_BELOW |  LADSPA_HINT_BOUNDED_ABOVE;
      m_PortHint[index].LowerBound = 0;//paraminfo.mMin;
      m_PortHint[index].UpperBound = 1;//paraminfo.mMax;
      index++;
    }

    // descriptor

    h_Memset(&m_LadspaDescr,0,sizeof(m_LadspaDescr));
    m_LadspaDescr.UniqueID            = m_Descriptor->m_UniqueId;
    m_LadspaDescr.Label               = m_Descriptor->m_Name.ptr();
    m_LadspaDescr.Properties          = LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
    m_LadspaDescr.Name                = m_Descriptor->m_Name.ptr();
    m_LadspaDescr.Maker               = m_Descriptor->m_Author.ptr();
    m_LadspaDescr.Copyright           = m_Descriptor->m_Product.ptr();
    m_LadspaDescr.PortCount           = m_Descriptor->m_NumInputs + m_Descriptor->m_NumOutputs + m_Descriptor->m_Parameters.size();
    m_LadspaDescr.PortDescriptors     = m_PortDesc;
    m_LadspaDescr.PortNames           = m_PortNames;
    m_LadspaDescr.PortRangeHints      = m_PortHint;
    m_LadspaDescr.ImplementationData  = this;
    m_LadspaDescr.instantiate         = lad_instantiate_callback;
    m_LadspaDescr.connect_port        = lad_connect_port_callback;
    m_LadspaDescr.activate            = lad_activate_callback;
    m_LadspaDescr.run                 = lad_run_callback;
    m_LadspaDescr.run_adding          = NULL; //lad_run_adding_callback;
    m_LadspaDescr.set_run_adding_gain = NULL; //lad_set_run_adding_gain_callback;
    m_LadspaDescr.deactivate          = lad_deactivate_callback;
    m_LadspaDescr.cleanup             = lad_cleanup_callback;

    return &m_LadspaDescr;

    //return 0;
  }

LADSPA_Handle h_Format::lad_instantiate(const LADSPA_Descriptor* Descriptor, unsigned long SampleRate)
  {
    h_Host* host = new h_Host();
    h_Instance* inst = createInstance(host,m_Descriptor);
    //inst->mSampleRate = SampleRate;
    inst->do_HandleState(is_Open);
    return (LADSPA_Handle)inst;
  }

//----------------------------------------------------------------------
//#endif
#endif





// axonlib instance descriptor:
/*
  // setup parameters
  for (int i=0; i<mDescriptor->getNumParams(); i++)
  {
    axParamInfo p = mDescriptor->getParamInfo(i);

    float val = p.mDef;
    if (val<0.33) val=0.25;
    else if (val>0.66) val=0.75;
    else val=0.5;
    switch(p.mType)
    {
      case pa_Par:
        appendParameter( new axParameter( this,p.mName,"", val ) );
        break;
      case pa_Float:
        appendParameter( new parFloat(    this,p.mName,"",val, p.mMin, p.mMax, p.mStep ) );
        break;
      case pa_Pow:
        appendParameter( new parFloatPow( this,p.mName,"",val, p.mMin, p.mMax, p.mStep, p.mAux ) );
        break;
      case pa_Int:
        appendParameter( new parInteger(  this,p.mName,"",val, p.mMin, p.mMax, p.mStr ) );
        break;
      case pa_Ctm:
        appendParameter( new parFloatCustom( this,p.mName,"",p.mDef, p.mMin, p.mMax, p.mStep, p.mPtr ) );
        break;
    }
  }
  setupParameters();
*/
