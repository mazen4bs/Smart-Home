#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdio.h> 
#include "UART.h"
#include "DIO.h"
#include "lamp.h"
#include "plug.h"
#include "door.h"
#include "temp.h"

int main() {
    UART0_Init();
    lamp_init();
    plug_init();
    door_init();
    ADC_Init();
    int count = 0;

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

        if (count == 16000-1){
          temp_update();
          count = 0;
        }

    }
}