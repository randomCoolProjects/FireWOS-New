#include <stdio.h>
#include <stdbool.h>

#ifndef IO_H
#define IO_H

#define INPUT_SERIAL 0
#define INPUT_KEYBOARD 1

#define INPUT_BUFSIZE 256

typedef char (*inputRead)();

inputRead iRead;
void memoryDump(void* addr, int length);
int parseHex(char* str);
void input_task(void*);
char* __linebuf;
int getline(char*);

#endif