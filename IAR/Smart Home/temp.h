#ifndef TEMP_H
#define TEMP_H

#include <stdint.h>

// Function prototypes
void ADC_Init(void);
uint32_t ADC_Read(void);
float Get_Temperature(void);
void temp_update(void);

#endif // ADC_H