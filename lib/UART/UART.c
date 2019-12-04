#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "UART.h"

void UartInit()
{
  fwos_uart_0 = uartBegin(UART_NUM, UART_BAUD, UART_CONFIG, UART_TX, UART_RX, UART_QUEUE_LEN, false);
}

void UartWrite(char* str)
{
  uartWriteBuf(fwos_uart_0, (const uint8_t *)str, strlen(str));
}

void UartBinWrite(char* str, int len)
{
  uartWriteBuf(fwos_uart_0, (const uint8_t *)str, len);
}

void UartPrintf(const char* fmt, ...)
{
    va_list    args;

    va_start(args, fmt);
    char buf[1024];
    vsprintf(buf, fmt, args);
    UartWrite(buf);
    va_end(args);
}

char UartRead()
{
  return uartRead(fwos_uart_0);
}

bool UartAvailable()
{
  return uartAvailable(fwos_uart_0);
}

char UartGetChar()
{
  while(!UartAvailable());
  return UartRead();
}

void UartFlush()
{
  uartFlush(fwos_uart_0);
}
