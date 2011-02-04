#include "stdio.h"
#include "lib/h_ComplexNumber.h"

int main(void)
{
  cnum_s t = {2, 1.5};
  t = cnum_pow(t, 2);
  printf("%f %f\n", t.r, t.i);
  return 0;
}
