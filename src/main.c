//#define _DEBUG_
#include <code_tools.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include <UART.h>
#include <Shell.h>
#include <Time.h>

ShellContext_t* shell;

#define DIR "/"

void loop(void)
{
  char* line = malloc(256);
  while (1)
  {
      UartWrite("\nFireWOS$ " DIR ">");
      while (getline(line) == -1);
      ShellExecute(shell, line);
      UartWrite("\n");
      UartWrite(shell->buf);
      memset(shell->buf, 0, SHELL_BUZ_SIZE);
      memset(line, 0, 256);
  }
  free(line); /*This never will happen, but ...*/
}

void app_main()
{
    UartInit();
    iRead = &UartGetChar;
    float boot = esp_timer_get_time() / 0.000001;
    printf("System booted in %f seconds.", boot);
    printf("<STDIO> Working...\n");
    DEBUG("Debug is working");
    shell = InitShell();
    xTaskCreate(input_task, "ipt_svc", 2048, NULL, 0, NULL);
    xTaskCreate(loop, "loopfunc",2048, NULL, 0, NULL);
    xTaskCreate(time_task, "time_svc",4096, NULL, 0, NULL);
}
