#include "tm4c123gh6pm.h"
#include "UART.h"
#include "DIO.h"
#include "door.h"

#define SWITCH_PORT 'F'
#define SWITCH_PIN Pin4
#define LED_PORT 'F'
#define LED_PIN Pin1

void door_init() {
    dio_init(SWITCH_PORT, SWITCH_PIN, Input); 
    dio_init(LED_PORT, LED_PIN, Output);       

    dio_writepin(LED_PORT, LED_PIN, 0);

    dio_writepin(SWITCH_PORT, SWITCH_PIN, 1); 
}

void door_update() {
    static uint8_t prev_state = 1;
    uint8_t current_state = dio_readpin(SWITCH_PORT, SWITCH_PIN);

    if (current_state != prev_state) {
        if (current_state == 0) {
            dio_writepin(LED_PORT, LED_PIN, 1);
            UART0_TransmitString("Door :Opened\r\n");
        } else {
            dio_writepin(LED_PORT, LED_PIN, 0);
            UART0_TransmitString("Door :Closed\r\n");
        }
        prev_state = current_state;
    }
}
