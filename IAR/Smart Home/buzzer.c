#include "tm4c123gh6pm.h"
#include "buzzer.h"
#include "temp.h"
#include "DIO.h"

#define TEMP_THRESHOLD 27  // Temperature threshold in °C

#define BUZZER_Port 'E'
#define BUZZER_PIN Pin1


void buzzer_init(void) {
    dio_init(BUZZER_Port, BUZZER_PIN, Output);
}

void buzzer_update(void) {
    if (Get_Temperature() > TEMP_THRESHOLD) {
      dio_writepin(BUZZER_Port, BUZZER_PIN, 1);
    } else {
      dio_writepin(BUZZER_Port, BUZZER_PIN, 0);
    }
}