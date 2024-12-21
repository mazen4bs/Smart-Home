#include <stdio.h> 
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "temp.h"

// ADC Initialization
void ADC_Init(void) {
    // Enable ADC0 and Port E
    SYSCTL_RCGCADC_R |= (1 << 0);  // Enable clock for ADC0
    SYSCTL_RCGCGPIO_R |= (1 << 4); // Enable clock for Port E

    // Wait for the peripherals to be ready
    while ((SYSCTL_PRADC_R & (1 << 0)) == 0);
    while ((SYSCTL_PRGPIO_R & (1 << 4)) == 0);

    // Configure PE3 (AIN0) as input
    GPIO_PORTE_AFSEL_R |= (1 << 3);  // Enable alternate function on PE3
    GPIO_PORTE_DEN_R &= ~(1 << 3);   // Disable digital function on PE3
    GPIO_PORTE_AMSEL_R |= (1 << 3);  // Enable analog function on PE3

    // Configure ADC0
    ADC0_ACTSS_R &= ~(1 << 3);       // Disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000;          // Configure for processor trigger
    ADC0_SSMUX3_R = 0;               // Set input to AIN0 (PE3)
    ADC0_SSCTL3_R = 0x6;             // Enable interrupt and set end of sequence
    ADC0_ACTSS_R |= (1 << 3);        // Enable sample sequencer 3
}

// ADC Read Function
uint32_t ADC_Read(void) {
    ADC0_PSSI_R = (1 << 3);          // Start a conversion on sample sequencer 3
    while ((ADC0_RIS_R & (1 << 3)) == 0);  // Wait for conversion to complete
    uint32_t result = ADC0_SSFIFO3_R;      // Read the conversion result
    ADC0_ISC_R = (1 << 3);           // Clear the interrupt flag
    return result;
}

// Convert ADC value to Temperature
float Get_Temperature(void) {
    uint32_t adcValue = ADC_Read();
    float voltage = (adcValue * 3.3) / 4096;  // Convert ADC value to voltage
    float temperature = voltage * 100;       // LM35: 10mV/°C = 0.01V/°C
    return temperature;
}

void temp_update(void){
     float temperature = Get_Temperature();
     char buffer[50];
     sprintf(buffer, "Temp : %.2f\r\n", temperature);
     UART0_TransmitString(buffer);

}
