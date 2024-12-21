#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

void UART0_Init(void);
void UART0_TransmitChar(char c);
char UART0_ReceiveChar(void);
void UART0_TransmitString(const char *str);

#endif