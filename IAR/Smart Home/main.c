#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdio.h> 
#include "UART.h"
#include "DIO.h"
#include "lamp.h"
#include "plug.h"
#include "door.h"
#include "temp.h"
#include "buzzer.h"


void Timer0A_Init(void) {
    SYSCTL_RCGCTIMER_R |= (1 << 0);          // Enable clock for Timer 0
    TIMER0_CTL_R &= ~0x1;                    // Disable Timer A during setup
    TIMER0_CFG_R = 0x0;                      // Configure for 32-bit timer
    TIMER0_TAMR_R = 0x2;                     // Configure for periodic mode
    TIMER0_TAILR_R = 32000000 - 1;           // Load value for 2-second delay (assuming 16 MHz clock)
    TIMER0_IMR_R |= (1 << 0);                // Enable Timer A timeout interrupt
    TIMER0_CTL_R |= 0x1;                     // Enable Timer A
    NVIC_EN0_R |= (1 << 19);                 // Enable interrupt 19 (Timer 0A)
}

void Timer0A_Handler(void) {
    TIMER0_ICR_R = (1 << 0);

    temp_update();
    buzzer_update();
}

int main() {
    UART0_Init();
    lamp_init();
    plug_init();
    door_init();
    ADC_Init();
    buzzer_init();
    Timer0A_Init();
    uint32_t count = 0;

    while (1) {
        count++;
        if ((UART0_FR_R & (1 << 4)) == 0) {
            char command = UART0_ReceiveChar();

            switch (command) {
            case 'L': case 'C':
                lamp_control(command);
                break;
            case 'I': case 'O':
                plug_control(command);
                break;
            default:
                UART0_TransmitString("Invalid command.\r\n");
                break;
            }
        }

        lamp_update();
        plug_update();
        door_update();
    }
}