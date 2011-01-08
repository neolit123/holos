#define H_FORMAT_EXE
#define H_DEBUG
#define H_DEBUG_LOG "_test.log"
#define H_DEBUG_CON
#define H_DEBUG_CON_CANQUIT
//#define H_DEBUG_CON_NOTOP
//#define H_DEBUG_CON_NORESIZE
#define H_DEBUG_MEM
#define H_DEBUG_NEW

#include "core/h_Debug.h"

#include "stdlib.h"

int main(void)
{
  h_Debug debug;
  
  int* j = (int*)malloc(100);
  int* p = new int[5];
  
  int a = 1;
  while (a)
  {
    dtrace("\n");
    trace("trace ok");
    dtrace("\n");
  }
  
  free(j);
  delete[] p;
  
  return 0; 
}
