#ifndef SHELL_H_
#define SHELL_H_

#include <inttypes.h>
#include <stdarg.h>

#ifndef SHELL_VAR_NUM
#define SHELL_VAR_NUM 32
#endif

#ifndef SHELL_FUNC_NUM
#define SHELL_FUNC_NUM 32
#endif

#ifndef SHELL_NAME_LEN
#define SHELL_NAME_LEN 16
#endif

#ifndef SHELL_BUF_SIZE
#define SHELL_BUZ_SIZE 1024
#endif

#define VALUE_STR 0x00
#define VALUE_BIN 0x01

//typedef struct ShellVariable_t;
//typedef struct

typedef void (*ShellFuncRaw_t)(int, char**);

typedef struct
{
  ShellFuncRaw_t function;
  char Name[SHELL_NAME_LEN];
} ShellFunc_t;

ShellFunc_t GlobalFunctions[SHELL_VAR_NUM];

typedef struct
{
  uint8_t ValueType;
  char* Value;
  uint32_t ValueLength;
  char Name[SHELL_NAME_LEN];
} ShellVariable_t;

typedef struct
{
  char buf[SHELL_BUZ_SIZE];
  ShellVariable_t Variables[SHELL_VAR_NUM];
  ShellFunc_t     Functions[SHELL_FUNC_NUM];
} ShellContext_t;
ShellContext_t* currShell;

int ShellStringPreProcess(char* src, char* buf, int len);

ShellContext_t* InitShell();

ShellVariable_t* GetVariable(ShellContext_t* ctx, char* name);

ShellFunc_t* GetFunction(ShellContext_t* ctx, char* name);

char* GetValue(ShellContext_t* ctx, char* str);

void ShellExecute(ShellContext_t* ctx, char* cmd);
void SplitCommands(char*formattedCmd);
//#define shPrintf(fmt, ...) sprintf(currShell->buf, fmt, __VA_ARGS__)
void shPrintf(const char* fmt, ...);
#endif
