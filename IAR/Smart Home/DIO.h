#ifndef DIO_H
#define DIO_H

#include <stdint.h>
#include "tm4c123gh6pm.h"

#define Input  0
#define Output 1

#define Pin0   (1U << 0)
#define Pin1   (1U << 1)
#define Pin2   (1U << 2)
#define Pin3   (1U << 3)
#define Pin4   (1U << 4)
#define Pin5   (1U << 5)
#define Pin6   (1U << 6)
#define Pin7   (1U << 7)

#define GPIO_PORTA_BASE  0x40004000
#define GPIO_PORTB_BASE  0x40005000
#define GPIO_PORTC_BASE  0x40006000
#define GPIO_PORTD_BASE  0x40007000
#define GPIO_PORTE_BASE  0x40024000
#define GPIO_PORTF_BASE  0x40025000

#define GPIO_PORT_DIR_R(port_base)       (*(volatile uint32_t *)(port_base + 0x400))
#define GPIO_PORT_DEN_R(port_base)       (*(volatile uint32_t *)(port_base + 0x51C))
#define GPIO_PORT_PUR_R(port_base)       (*(volatile uint32_t *)(port_base + 0x510))
#define GPIO_PORT_LOCK_R(port_base)      (*(volatile uint32_t *)(port_base + 0x520))  
#define GPIO_PORT_CR_R(port_base)        (*(volatile uint32_t *)(port_base + 0x524))  
#define GPIO_PORT_DATA_BITS_R(port_base) ((volatile uint32_t *)(port_base))         

void dio_init(char port, uint8_t pins, uint8_t direction);
uint8_t dio_readpin(char port, uint8_t pin);
uint8_t dio_readport(char port);
void dio_writepin(char port, uint8_t pin, uint8_t value);
void dio_writeport(char port, uint8_t value);

#endif
