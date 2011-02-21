#include "h/h_File.h"

int main(void)
{
  h_File tFile;
  short wResult;
  char* fName = (char*)"./someFile.bin";
  char* wBuffer = (char*)"Hello World!";
  char* rBuffer[256];
  
  wResult = tFile.write(fName, wBuffer, H_FILE_WB, 12);
  trace("[tFile.write]");
  trace("wResult: " << wResult);

  tFile.read(fName, rBuffer, H_FILE_RB);
  trace("[tFile.read]");
  trace("tFile.m_Size:" << tFile.m_Size);
  trace("rBuffer:" << rBuffer);

  return 0;
}
