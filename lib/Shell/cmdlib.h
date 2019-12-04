#ifndef CMDLIB_H
#define CMDLIB_H
//Just a shell command library, can be in a H file only
#include <Shell.h>
#include <code_tools.h>
#include <stdio.h>
#include <string.h>
#include <Time.h>

typedef struct
{
  void* function;
  char* name;
} funcInfo_t;

void shellPrint(int argc, char** args)
{
  if (argc == 0)
  {
    shPrintf("\n");
  }
  else
  {
    DEBUG("<Shell> print() ...\n");
    for (int i = 0; i < argc; i++)
    {
      char* vptr = args[i];
      if (!vptr) continue;
      shPrintf("%s ", vptr);
    }
    shPrintf("\n");
  }
}

void timeView(int argc, char** args)
{
  if (mainTime)
  shPrintf("\nCurrent Time is: \n%d/%d/%d - %d:%d:%d\n", mainTime->day, mainTime->month, mainTime->year, mainTime->hour, mainTime->minute, mainTime->second);
  else
  {
    shPrintf("Error\n");
  }
  
}

funcInfo_t Functions_lib[] = {
  {&shellPrint, "print"},
  {&timeView,   "time"}
};

#endif