#include <stdint.h>
#include "params.h"
#include "rounding.h"
int32_t power2round(int32_t *a0, int32_t a)  {
  int32_t a1;
  int32_t a0tmp;
  int32_t powerd = 1 << D;
  int32_t halfpowerd = powerd >> 1;
  // a1 = (a + (1 << (D-1)) - 1) >> D;
  // *a0 = a - (a1 << D);
  // return a1;  // a = a % Q;
  if(a<0)
  {
    a = a+Q;
  }
  a0tmp = a % powerd;
  if(a0tmp > halfpowerd)
  {
    a0tmp = a0tmp - powerd;
  }
  a1 = a - a0tmp;
  a1 = a1 >> D;

  *a0 = a0tmp;
  return a1;
}

int32_t decompose(int32_t *a0, int32_t a) {
  int32_t a1;

  a = a % Q;
  if (a < 0)
	  a += Q;
  *a0 = a % (2 * GAMMA2);
  if (*a0 > GAMMA2)
	  *a0 -= (2*GAMMA2);
  if (a - (*a0) == (Q - 1)) 
  {
	  a1 = 0;
	  *a0 = *a0 - 1;
  }
  else
  {
	  a1 = (a - (*a0)) / (2 * GAMMA2);
  }
  return a1;
}

unsigned int make_hint(int32_t a0, int32_t a1) { // a0:w-cs2+ct0의 lowbits, a1:w-cs2의 highbits
  if(a0 > GAMMA2 || a0 < - GAMMA2 || (a0 == -GAMMA2 && a1 != 0)) //if(a0 <= GAMMA2 || a0 > Q - GAMMA2 || (a0 == Q - GAMMA2 && a1 == 0)) //Q는 왜 있지?-> ct0더할때 modQ로 양수로 만들어버렸으니까
    return 1;

  return 0;
}

int32_t use_hint(int32_t a, unsigned int hint) {
  int32_t a0, a1;
  
  a1 = decompose(&a0, a);
  if(hint == 0)
    return a1;
#if N==1152
  if(a0 > 0)
    return (a1 == 31) ?  0 : a1 + 1;
  else
    return (a1 ==  0) ? 31 : a1 - 1;
#elif N==1536
  if(a0 > 0)
    return (a1 == 31) ?  0 : a1 + 1;
  else
    return (a1 ==  0) ? 31 : a1 - 1;
#elif N==2304
  if(a0 > 0)
    return (a1 == 15) ?  0 : a1 + 1;
  else
    return (a1 ==  0) ? 15 : a1 - 1;
#endif

#if N==1021
  if(a0 > 0)
    return (a1 == 44) ?  0 : a1 + 1;
  else
    return (a1 ==  0) ? 44 : a1 - 1;
#elif N==1429
  if(a0 > 0)
    return (a1 == 27) ?  0 : a1 + 1;
  else
    return (a1 ==  0) ? 27 : a1 - 1;
#elif N==1913
  if(a0 > 0)
    return (a1 == 20) ?  0 : a1 + 1;
  else
    return (a1 ==  0) ? 20 : a1 - 1;
#endif
}
