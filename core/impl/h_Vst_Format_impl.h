#ifndef h_Vst_Format_impl_included
#define h_Vst_Format_impl_included
#ifdef h_Vst_included
//----------------------------------------------------------------------

h_Format::h_Format()
  {
    m_Initialized = false;
  }

//----------

h_Format::~h_Format()
  {
  }

//----------------------------------------

//h_Descriptor* h_Format::getDescriptor(void)
//  {
//    return &H_DESCRIPTOR;
//  }

h_Descriptor* h_Format::createDescriptor(void)
  {
    return new H_DESCRIPTOR();
  }

//----------

h_Instance* h_Format::createInstance(h_Host* a_Host, h_Descriptor* a_Descriptor)
  {
    return new H_INSTANCE(a_Host, a_Descriptor);
  }

//----------------------------------------

// this is called for each instance of the vst plugins

AEffect* h_Format::entrypoint(audioMasterCallback audioMaster)
  {
    trace("h_Format::entrypoint");
    AEffect*      effect  = (AEffect*)h_Malloc( sizeof(AEffect) );
    h_Host*       host    = new h_Host(audioMaster,effect);
    //h_Descriptor* desc    = getDescriptor();
    h_Descriptor* desc    = createDescriptor();
    h_Instance*   inst    = createInstance(host,desc);

    // instance must remember the above effect and host objects,
    // (read them in its constructor)
    // and delete them in its destructor..

    //h_Strcpy(m_RealName,desc->m_Name);
    //#ifdef H_DEBUG
    //  h_Strcat(m_RealName,(char*)"_debug");
    //#endif

    h_Memset(effect,0,sizeof(effect));
    effect->magic                   = kEffectMagic;
    effect->object                  = inst;                   //
    effect->user                    = this;                   // ???
    effect->dispatcher              = vst_dispatcher_callback;
    effect->setParameter            = vst_setParameter_callback;
    effect->getParameter            = vst_getParameter_callback;
    effect->processReplacing        = vst_processReplacing_callback;
    effect->processDoubleReplacing  = vst_processDoubleReplacing_callback;
    effect->flags                   = effFlagsCanReplacing;
    effect->version                 = 0;
    effect->uniqueID                = desc->m_UniqueId;//H_MAGIC + 0x0000;
    //effect->numPrograms             = desc->m_NumProgs;
    //effect->numParams               = 0;//desc->m_NumParams;
    effect->numPrograms             = 0;
    effect->numParams               = desc->m_Parameters.size();
    effect->numInputs               = desc->m_NumInputs;
    effect->numOutputs              = desc->m_NumOutputs;
    effect->initialDelay            = 0;
    if (desc->m_Flags & df_HasEditor) effect->flags |= effFlagsHasEditor;
    if (desc->m_Flags & df_IsSynth)   effect->flags |= effFlagsIsSynth;
    trace("...entrypoint ok");
    return effect;
  }

//----------------------------------------------------------------------
// callbacks
//----------------------------------------------------------------------

// static callbacks (called by vst-host)
// pass the call over to the proper instance (AEffect.object)

//static
VstIntPtr h_Format::vst_dispatcher_callback(AEffect* ae, VstInt32 opCode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
  {
    h_Instance* inst = (h_Instance*)ae->object;
    if (opCode==effClose)
    {
      inst->vst_dispatcher(opCode,index,value,ptr,opt);
      //delete inst->getAEffect();
      delete inst;
      return 1;
    }
    return inst->vst_dispatcher(opCode,index,value,ptr,opt);
  }

//----------

//static
float h_Format::vst_getParameter_callback(AEffect* ae, VstInt32 index)
  {
    h_Instance* inst = (h_Instance*)ae->object;
    return inst->vst_getParameter(index);
  }

//----------

//static
void h_Format::vst_setParameter_callback(AEffect* ae, VstInt32 index, float value)
  {
    h_Instance* inst = (h_Instance*)ae->object;
    inst->vst_setParameter(index,value);
  }

//----------

//static
void h_Format::vst_processReplacing_callback(AEffect* ae, float** inputs, float** outputs, VstInt32 sampleFrames)
  {
    h_Instance* inst = (h_Instance*)ae->object;
    inst->vst_processReplacing(inputs,outputs,sampleFrames);
  }

//----------

//static
void h_Format::vst_processDoubleReplacing_callback(AEffect* ae, double** inputs, double** outputs, VstInt32 sampleFrames)
  {
    h_Instance* inst = (h_Instance*)ae->object;
    inst->vst_processDoubleReplacing(inputs,outputs,sampleFrames);
  }

//----------------------------------------------------------------------
#endif
#endif

