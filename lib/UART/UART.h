#ifndef FIREWOS_UART_H
#define FIREWOS_UART_H 1

#include <Serial.h>

#ifndef UART_CONFIG_AUTO
#define UART_NUM 0
#define UART_TX 3
#define UART_RX 1
#define UART_CONFIG SERIAL_8N1
#define UART_BAUD 115200
#define UART_QUEUE_LEN 256
#endif

uart_t* fwos_uart_0;

void UartInit();

void UartWrite(char* str);

void UartBinWrite(char* str, int len);

void UartPrintf(const char* fmt, ...);

void UartFlush();

char UartRead();

bool UartAvailable();

char UartGetChar();

#endif
