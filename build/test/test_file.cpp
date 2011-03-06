// test h/h_File.h & log to home dir

#define H_DEBUG_CON
#define H_DEBUG_LOG_HOME
#define H_DEBUG_LOG "test_file.log"

#include "holos.h"
#include "src/h_File.h"

class desc : public h_Descriptor
{
  public:
    desc() : h_Descriptor()
    {
      m_Name        = "test_file";
      m_Author      = "na";
      m_Product     = "holos test plugin";
      m_Version     = 0;
      m_UniqueId    = H_MAGIC;
      m_Flags       = df_None;
      m_NumInputs   = 2;
      m_NumOutputs  = 2;
      m_EditorRect  = H_NULL_RECT;
      appendParameter( new h_Parameter("test_file", "test_file_l"));
    }
};

class inst : public h_Instance
{
  public:
    inst(h_Host* a_Host, h_Descriptor* a_Descriptor)
    : h_Instance(a_Host, a_Descriptor)
    {
      h_File  tFile;
      char*   fName = (char*)"./testFile.bin";
      char*   wBuffer = (char*)"Hello World!\0";
      long    fLength = 12;
      short   wResult;
      char*   rBuffer;

      // write
      wResult = tFile.write(fName, wBuffer, fLength, sizeof(char), H_FILE_WB);
      dtrace("wResult: " << wResult << "\n");

      // get length
      fLength = tFile.length(fName);
      dtrace("fLength:" << fLength << "\n");

      // manual malloc
      rBuffer = (char*)h_Malloc(fLength);
      tFile.read(fName, rBuffer, fLength, sizeof(char), H_FILE_RB);
      dtrace("tFile.m_Length:" << tFile.m_Length);
      dtrace("rBuffer:" << rBuffer << "\n");
      tFile.freebuf();

      // auto: get length and malloc      
      tFile.read(fName, rBuffer, H_FILE_RAUTO, sizeof(char), H_FILE_RB);
      dtrace("tFile.m_Length:" << tFile.m_Length);
      dtrace("rBuffer:" << rBuffer << "\n");
      tFile.freebuf();

    }
};

#define H_DESCRIPTOR  desc
#define H_INSTANCE    inst

#include "holos_impl.h"
