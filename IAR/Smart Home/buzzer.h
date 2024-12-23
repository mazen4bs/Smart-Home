#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

// Define the temperature threshold
#define TEMP_THRESHOLD 27  // Temperature threshold in °C

// Function Prototypes
void buzzer_init(void);      // Initialize the buzzer
void buzzer_update(void);    // Update the buzzer state based on temperature

#endif // BUZZER_H