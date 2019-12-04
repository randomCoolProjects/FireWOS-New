#ifndef CODE_TOOLS_H
#define CODE_TOOLS_H

#include <stdlib.h>
#include <UART.h>

#define arr_len(array) sizeof(array) / sizeof(array[0])

#define foreach(item, array) \
    for(int keep=1, \
            count=0,\
            size=sizeof (array)/sizeof *(array); \
        keep && count != size; \
        keep = !keep, count++) \
      for(item = (array)+count; keep; keep = !keep)

#define LOOP(array) for (int i = 0; i < sizeof(array) / sizeof(array[0]); i ++)

#define REPEAT(times) for(int i = 0; i < times; i ++)

#define CRASH while(1);

#ifdef _DEBUG_
#define DEBUG(msg) UartPrintf("{DEBUG}: " msg " - at %s, line %d.\n", __FILE__, __LINE__)
#define DEBUGF(fmt, ...) UartPrintf("{DEBUG}: " fmt " - at %s, line %d\n", __VA_ARGS__, __FILE__, __LINE__)
//#define DEBUGVAL(val, fmt) printf("{DEBUG} The value of " ## val ## " is " ## fmt, val)
#endif

#ifndef _DEBUG_
#define DEBUG(...)
#define DEBUGF(...)
#endif

#define CONST_CAT (s1, s2) s1 ## s2

typedef union
{
  char* sptr;
  void* vptr;
  int* iptr;
  float* fptr;
  double* dptr;
  int ptr_value;
} ptr_types;

/*ptr_types generic_alloc(int size)
{
  void* ptr = malloc(size);
  ptr_types p;
  p.vptr = ptr;
  return p;
}*/

#endif
