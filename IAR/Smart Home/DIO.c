#include "DIO.h"
#include "bitwise_operations.h"

uint8_t ctz(uint32_t value) {
    uint8_t count = 0;
    if (value == 0) return 32;
    while ((value & 1) == 0) {
        count++;
        value >>= 1;
    }
    return count;
}

void dio_init(char port, uint8_t pins, uint8_t direction) {
    uint32_t port_base = 0;

    switch (port) {
        case 'A': port_base = GPIO_PORTA_BASE; break;
        case 'B': port_base = GPIO_PORTB_BASE; break;
        case 'C': port_base = GPIO_PORTC_BASE; break;
        case 'D': port_base = GPIO_PORTD_BASE; break;
        case 'E': port_base = GPIO_PORTE_BASE; break;
        case 'F': port_base = GPIO_PORTF_BASE; break;
        default: return;
    }


    SYSCTL_RCGCGPIO_R |= (1U << (port - 'A'));
    while ((SYSCTL_PRGPIO_R & (1U << (port - 'A'))) == 0);

    if (port == 'A' && (pins & Pin0)) {
        GPIO_PORT_LOCK_R(port_base) = 0x4C4F434B;
        GPIO_PORT_CR_R(port_base) |= pins;       
    }
    if (port == 'C' && (pins & Pin0)) {  
        GPIO_PORT_LOCK_R(port_base) = 0x4C4F434B;  
        GPIO_PORT_CR_R(port_base) |= pins;        
    }
    if (port == 'D' && (pins & Pin7)) {  
        GPIO_PORT_LOCK_R(port_base) = 0x4C4F434B;  
        GPIO_PORT_CR_R(port_base) |= pins;        
    }
    if (port == 'F' && (pins & Pin0)) {
        GPIO_PORT_LOCK_R(port_base) = 0x4C4F434B;  
        GPIO_PORT_CR_R(port_base) |= pins;        
    }

    if (direction == Output) {
        GPIO_PORT_DIR_R(port_base) |= pins;
    } else if (direction == Input) {
        GPIO_PORT_DIR_R(port_base) &= ~pins;

        GPIO_PORT_PUR_R(port_base) |= pins;
    }

    GPIO_PORT_DEN_R(port_base) |= pins;
}

uint8_t dio_readpin(char port, uint8_t pin) {
    uint32_t port_base = 0;

    switch (port) {
        case 'A': port_base = GPIO_PORTA_BASE; break;
        case 'B': port_base = GPIO_PORTB_BASE; break;
        case 'C': port_base = GPIO_PORTC_BASE; break;
        case 'D': port_base = GPIO_PORTD_BASE; break;
        case 'E': port_base = GPIO_PORTE_BASE; break;
        case 'F': port_base = GPIO_PORTF_BASE; break;
        default: return 0;
    }

    return GET_BIT(*(volatile uint32_t *)(port_base + 0x3FC), ctz(pin));
}

uint8_t dio_readport(char port) {
    uint32_t port_base = 0;

    switch (port) {
        case 'A': port_base = GPIO_PORTA_BASE; break;
        case 'B': port_base = GPIO_PORTB_BASE; break;
        case 'C': port_base = GPIO_PORTC_BASE; break;
        case 'D': port_base = GPIO_PORTD_BASE; break;
        case 'E': port_base = GPIO_PORTE_BASE; break;
        case 'F': port_base = GPIO_PORTF_BASE; break;
        default: return 0;
    }

    return (uint8_t)(*(volatile uint32_t *)(port_base + 0x3FC));
}

void dio_writepin(char port, uint8_t pin, uint8_t value) {
    uint32_t port_base = 0;

    switch (port) {
        case 'A': port_base = GPIO_PORTA_BASE; break;
        case 'B': port_base = GPIO_PORTB_BASE; break;
        case 'C': port_base = GPIO_PORTC_BASE; break;
        case 'D': port_base = GPIO_PORTD_BASE; break;
        case 'E': port_base = GPIO_PORTE_BASE; break;
        case 'F': port_base = GPIO_PORTF_BASE; break;
        default: return;
    }

    if (value) {
        SET_BIT(*(volatile uint32_t *)(port_base + (1U << (2 + ctz(pin)))), ctz(pin));
    } else {
        CLEAR_BIT(*(volatile uint32_t *)(port_base + (1U << (2 + ctz(pin)))), ctz(pin));
    }
}

void dio_writeport(char port, uint8_t value) {
    uint32_t port_base = 0;

    switch (port) {
        case 'A': port_base = GPIO_PORTA_BASE; break;
        case 'B': port_base = GPIO_PORTB_BASE; break;
        case 'C': port_base = GPIO_PORTC_BASE; break;
        case 'D': port_base = GPIO_PORTD_BASE; break;
        case 'E': port_base = GPIO_PORTE_BASE; break;
        case 'F': port_base = GPIO_PORTF_BASE; break;
        default: return;
    }

    *(volatile uint32_t *)(port_base + 0x3FC) = value;
}
