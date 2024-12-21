#include "tm4c123gh6pm.h"
#include "lamp.h"
#include "DIO.h"

#define RELAY_PORT 'D'
#define RELAY_PIN Pin6

#define SWITCH_PORT 'D'
#define SWITCH_PIN Pin7

#define APP_PORT 'C'
#define APP_PIN Pin7

void lamp_init() {
    dio_init(RELAY_PORT, RELAY_PIN, Output);
    dio_init(SWITCH_PORT, SWITCH_PIN, Input);
    dio_init(APP_PORT, APP_PIN, Output);
}

void lamp_control(char command) {
    switch (command) {
    case 'L':
        dio_writepin(APP_PORT, APP_PIN, 1);
        break;
    case 'C':
        dio_writepin(APP_PORT, APP_PIN, 0);
        break;
    default:
        break;
    }
}

void lamp_update() {
    uint8_t app_state = dio_readpin(APP_PORT, APP_PIN);
    uint8_t switch_state = dio_readpin(SWITCH_PORT, SWITCH_PIN);

    uint8_t relay_state = app_state ^ switch_state;
    dio_writepin(RELAY_PORT, RELAY_PIN, relay_state);
}
