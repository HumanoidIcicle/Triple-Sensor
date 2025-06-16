/***************************************************************************
 * ADC Source File
 *
 * Created: 2025-05-29
 * Updated: 2025-05-29
 * Author: JOSS0001
 **************************************************************************/

/***** Include section ****************************************************/
#include "ADC_HeaderFile.h"

/*********** Functions ***********/
void setup_ADC(int channel) {
	ADMUX = (1 << REFS0); // AVcc reference
	if(channel== 1) ADMUX |= (1 << MUX0);
	else if(channel== 2) ADMUX |= (1 << MUX1);
	else if(channel== 3) ADMUX |= (1 << MUX2); 
	
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); // Prescaler 128
	while (ADCSRA & (1 << ADSC)); // Wait for conversion
}

float read_voltage() {
	return (ADC * 5.0f) / 1023; //Conversion of potentiometer value to voltage.
}

float read_temperature() {
	return (ADCW * 500.0f) / 1023; // Conversion from temp. sensor to degrees C.
}

float read_brightness() {
	return (ADC * 100.0f) / 1023; // Brightsness as percentage.
}
