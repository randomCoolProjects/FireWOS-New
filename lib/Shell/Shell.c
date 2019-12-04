#define _DEBUG_
#include "Shell.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <code_tools.h>
#include <security.h>
#include <UART.h>
#include "cmdlib.h"

void shPrintf(const char* fmt, ...)
{
  if (!currShell) {
    DEBUG("No shell context!");
    return;
  }
    va_list    args;

    va_start(args, fmt);
    char buf[256];
    vsprintf(buf, fmt, args);
    strcat(currShell->buf, buf);
    va_end(args);
}

ShellContext_t* InitShell()
{
  ShellContext_t* ctx = malloc(sizeof(ShellContext_t));
  LOOP(ctx->Variables)
  {
    ShellVariable_t* var = &ctx->Variables[i];
    var->ValueLength = 0;
  }
  memset(ctx->buf, 0, 1024);
  LOOP(Functions_lib)
  {
    funcInfo_t* info = &Functions_lib[i];
    ShellFuncRaw_t func = (ShellFuncRaw_t)info->function;
    ShellFunc_t s_func;
    strcpy(s_func.Name, info->name);
    s_func.function = func;
    ctx->Functions[i] = s_func;
  }
  return ctx;
}

ShellVariable_t* GetVariable(ShellContext_t* ctx, char* name)
{
  if (!ctx)
  {
    DEBUG("No shell context!");
    return NULL;
  }
  for (int i= 0; i < arr_len(ctx->Variables); i ++) {
    ShellVariable_t* var = &ctx->Variables[i];
    if (var && strcmp(var->Name, name) == 0)
    {
      return var;
    }
  }
  return NULL;
}

char* GetValue(ShellContext_t* ctx, char* str)
{
  ShellVariable_t* var = GetVariable(ctx, str);
  if (var)
  {
    int len = strlen(var->Value);
    char* sptr = (char*)calloc(len+1, 1);
    strcpy(sptr, var->Value);
    return sptr;
  }

  int size = strlen(str);
  char* sptr = calloc(size+1, 1);
  strcpy(sptr, str);
  return sptr;
}

//print "Hello, World" -> print;Hello World;
int ShellStringPreProcess(char* src, char* buf, int len)
{
  char* bufptr = buf;
  memset(buf, 0, strlen(buf));
  bool openstr = false;
  bool changed = false;
  char c;
  int i = 0;
  while(*src)
  {
    if (i++ > len) {
      *buf = ';';
      break;
    };
    char old = c;
    c = *src++;
    if (c == '\n' || c == '\r') continue;
    if (c == '"')
    {
      openstr = !openstr;
      if (!changed)continue;
      *buf++ = ';';
      changed = false;
      continue;
    }
    else if (c == ' ' && !openstr)
    {
      if (!changed)continue;
      *buf++ = ';';
      changed = false;
      continue;
    }
    *buf++ = c;
    changed = true;
  }
  *buf++ = ';';
  return strlen(bufptr);
}

char scmds[25][64];
int totalcmds = 0;
void SplitCommands(char* formattedCmd)
{
  totalcmds = 0;
  int idx = 0;
  int cidx = 0;
  while (*formattedCmd)
  {
    char c = *formattedCmd++;
    if (c == ';')
    {
      scmds[idx][cidx++] = 0x00;
      idx++;
      totalcmds++;
      cidx=0;
      continue;
    }
    scmds[idx][cidx++] = c;
  }
}

ShellFunc_t* GetFunction(ShellContext_t* ctx, char* name)
{
  LOOP(ctx->Functions){
    ShellFunc_t* func = &ctx->Functions[i];
    if (!func || !func->function) continue;
    if (strcmp(func->Name, name) == 0)
    {
      return func;
    }
  }
  return NULL;
}

char cmdbuf[512]; // A shell command line will not have more than 512 characters!
bool exScript_runing = false;
void ShellExecute(ShellContext_t* ctx, char* cmd)
{
  //while(exScript_runing);
  exScript_runing = true;

  currShell = ctx;
  int len = strlen(cmd);
  if (len <= 0 || strcmp(cmd, "\n") == 0) {
    goto end;
    exScript_runing = false;
    return;
  }
  memset(cmdbuf, 0, 512);
  int plen = ShellStringPreProcess(cmd, cmdbuf, len);

  for (int i= 0; i < 25; i ++)
  {
    memset(scmds[i], 0, 64);
  }

  SplitCommands(cmdbuf);

  #define ARG(num) scmds[num]
  char* fname = ARG(0);

  if (strcmp(ARG(0), "set") == 0) // set <variable> <value>
  {
    ShellVariable_t* var = GetVariable(ctx, ARG(1));
    char* value = GetValue(ctx, ARG(2));
    int vlen = strlen(value);
    if (var)
    {
      var->Value = realloc(var->Value, vlen+1);
      memset(var->Value, 0, vlen+1);
      strcpy(var->Value, value);
      var->ValueLength = vlen;
    }
    else
    {
      LOOP(ctx->Variables)
      {
        ShellVariable_t* shvar = &ctx->Variables[i];
        DEBUGF("Searching var, I = %d, vname = %s", i, (shvar->Name == NULL ? "NULL" : shvar->Name));
        if (!shvar)
        {
          UartWrite("Memory corrupted.\n"); CRASH
        }
        if (strlen(shvar->Name) == 0 || shvar->Value == NULL || shvar->ValueLength == 0)
        {
          ShellVariable_t newvar;
          DEBUGF("The name is (ARG(1) = %s)\n", ARG(1));
          strcpy(newvar.Name, ARG(1));
          newvar.Value = calloc(vlen+1, 1);
          strcpy(newvar.Value, value);
          newvar.ValueLength = vlen;
          newvar.ValueType = VALUE_STR;
          *shvar = newvar;
          //ctx->Variables[i] = newvar;
          goto end;
        }
      }
    }
    goto end;
  }

  DEBUG("trying to find a variable for");
  {
    ShellVariable_t* svar = GetVariable(ctx, fname);
    if (svar)
    {
      shPrintf("%s", svar->Value);
      goto end;
    }
    DEBUG("No variable found");
  }

  DEBUG("trying to find a function");
  {
    ShellFunc_t* sfunc = GetFunction(ctx, fname);
    if (sfunc != NULL)
    {
      DEBUG("function found");
      char* values[24];

      for (int i = 1; i < totalcmds; i ++)
      {
        char* str = GetValue(ctx, ARG(i));
        values[i-1] = str;
      }

      char** ptr = values;

      DEBUG("calling function()");
      if (!sfunc->function)
      {
        DEBUG("Error ...");
        while(1);
      }
      sfunc->function(totalcmds-1, ptr);

      for (int i = 1; i < totalcmds; i ++)
      {
        free(values[i-1]);
      }

      goto end;
    }
    shPrintf("No matching function for %s.\n", fname);
    goto end;
  }

  end: ;
  exScript_runing = false;
}
