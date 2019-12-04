#include "io.h"
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <UART.h>

void memoryDump(void* addr, int length)
{
    uint8_t* ptr = (uint8_t*)addr;
    printf("Dumping physical memory at %p\n", ptr);
    int bl = 16;
    int bc = 0;

    char str[bl+1];
    int s_index = 0;

    for (int i = 0; i < length; i ++)
    {
        if (bc == bl)
        {
            printf("    %s\n", str);
            memset(str, 0, bl);
            s_index = 0;
            bc=0;
        }
        uint8_t val = (uint8_t)(*ptr);
        if (val >= 32 && val <= 126 && val != '\n' && val != '\r')
        str[s_index] = (char)val;
        else
        {
            str[s_index] = '.';
        }

        s_index++;
        
        printf("%02X ", val);
        ptr++;
        bc++;
    }

    if (bc != 0)
    {
        for (int i = bc; i < bl; i ++)
        printf("   ");
        printf("    %s\n", str);
    }

    printf("\nFinished at %p\n\n", ptr);
}

int parseHex(char* str)
{
  return (int) strtol(str, 0, 16);
}

char* BUFFER = NULL;
int getline(char* buf)
{
    if (BUFFER != NULL)
    {
        int len = strlen(BUFFER);
        memcpy(buf, BUFFER, len);

        memset(BUFFER, 0, len);
        BUFFER = NULL;
        return len;
    }
    return -1;
}

char input_type = INPUT_KEYBOARD; /*plataformio monitor simulates a keyboard input by serial*/
void input_task(void* out)
{
    __linebuf = calloc(INPUT_BUFSIZE, 1);
    int ct = 0;
    UartPrintf("Input service started with IPT_MODE = %04x", input_type);
    while(1)
    {
        char c = iRead();
        if (c == 0x00 || BUFFER != NULL) continue;
        if (c == '\n' || c == '\r' || ct+1 == INPUT_BUFSIZE)
        {
            BUFFER = __linebuf;
            ct = 0;
            continue;
        }
        if (input_type == INPUT_KEYBOARD)
            UartBinWrite(&c, 1);
        __linebuf[ct++] = c;
    }
}