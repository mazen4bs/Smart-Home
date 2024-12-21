#include "tm4c123gh6pm.h"
#include "plug.h"
#include "DIO.h"

#define RELAY_PORT 'F'
#define RELAY_PIN Pin2

#define SWITCH_PORT 'A'
#define SWITCH_PIN Pin2

#define APP_PORT 'A'
#define APP_PIN Pin6

void plug_init() {
    dio_init(RELAY_PORT, RELAY_PIN, Output);
    dio_init(SWITCH_PORT, SWITCH_PIN, Input);
    dio_init(APP_PORT, APP_PIN, Output);
}

void plug_control(char command) {
    switch (command) {
    case 'I':
        dio_writepin(APP_PORT, APP_PIN, 1);
        break;
    case 'O':
        dio_writepin(APP_PORT, APP_PIN, 0);
        break;
    default:
        break;
    }
}

void plug_update() {
    uint8_t app_state = dio_readpin(APP_PORT, APP_PIN);
    uint8_t switch_state = dio_readpin(SWITCH_PORT, SWITCH_PIN);

    uint8_t relay_state = app_state ^ switch_state;
    dio_writepin(RELAY_PORT, RELAY_PIN, relay_state);
}
