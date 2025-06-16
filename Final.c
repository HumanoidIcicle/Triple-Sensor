/*
 * DT124A Final Project.c
 *
 *Triple Sensor Monitoring Program
 *
 * Created: 2025-05-24 12:23:04
 * Last Updated: 2025-05-31
 * Author : JOSS0001
 */

/*
Program that reads environmental data from three sensors connected to analog pins 1, 2, and 4. 
A sensor to monitor is selected using 3 buttons connected to digital pins 1, 2, and through. 
Through the use of external interrupts, the program is able to switch between the sensors and 
display the value of the selected sensor on LCD.
*/


/* Definition of constants and CPU frequency */
#define F_CPU 16000000
#define WARNING 15625
#define FATAL 3900

/* Inclusion of relevant libraries */
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.c"
#include "ADC.c"


volatile int choice = 0; //Global variable to be changed in ISRs

/* Function to enable the interrupts on used pins */
void set_Interrupts() {
	PCICR = (1 << PCIE2);
	PCMSK2 = (1 << PCINT17);

	EICRA |= (1 << ISC01) | (1 << ISC00) | (1 << ISC11) | (1 << ISC10);
	EIMSK |= (1 << INT0) | (1 << INT1);
	sei();
}

/* Function to properly display sensor values on LCD */
void display_values(float value, uint8_t* str_value, char* measured, char* unit) {
	dtostrf(value, 6, 3, str_value);
	lcd_clear_row(FIRST_ROW);
	lcd_clear_row(SECOND_ROW);
	lcd_putcmd(LCD_SET_CURSOR | FIRST_ROW);
	lcd_puts(measured);
	lcd_putcmd(LCD_SET_CURSOR | SECOND_ROW);
	lcd_puts(str_value);
	lcd_puts(unit);
	_delay_ms(50);
}

/* Function to configure the timer in CTC mode to enable the OCR1A*/
void set_Timer() {
	DDRD |= (1 << PIND4) | (1 << PIND5) | (1 << PIND6) | (1 << PIND7);
	TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12); // prescaler 1024
}

/* Function to toggle LED on argument pin and buzzer on pin 4 */
void system_status(uint16_t pin) {
	if (TIFR1 & (1 << OCF1A)) { // Check if compare flag is set
		PORTD ^= ((1 << pin) | (1 << PIND4));
		TIFR1 |= (1 << OCF1A);  // Clear the flag
	}
}

/* ISRs for interrupt on pins 1, 2, 3 */
ISR(PCINT2_vect) { 
	if (!(PIND & (1 << PIND1))) choice = 1; // Button 1 had been pressed 
}
ISR(INT0_vect) {
	choice = 2; // Button 2 had been pressed 
}
ISR(INT1_vect) {
	choice = 3; // Button 3 had been pressed 
}

int main(void) {
	lcd_init();
	set_Interrupts();
	set_Timer();

	while (1) {
		/* Create variables to store the converted ADC values */
		float value; // As float to make comparisons
		unsigned char str_value[10]; // As strings to be written to LCD

		setup_ADC(choice); // Function defined in ADC.c, Starts ADC enables analog pins according to sensor selected
		
		/* Choice 1 to read from potentiometer */
		if (choice == 1) { 
			value = read_voltage(); // Initialize value to the voltage conversion
			display_values(value, str_value, "Source Voltage: ", " V");

			if (value < 3) PORTD = ((1 << PIND7) | (0 << PIND4)); // Normal range for voltage, light the blue LED
			else if (value > 3 && value < 4) { // Range to set off warning buzzer and LED
				OCR1A = WARNING;
				PORTD &= ~((1 << PIND5) | (1 << PIND7)); // Turnoff the blue LED
				system_status(PIND6);
			}
			else { // Range to set off critical alert buzzer and LED
				OCR1A = FATAL;
				PORTD &= ~((1 << PIND6) | (1 << PIND7));
				system_status(PIND5);
			}
		}
		/* Choice 2 to read from temperature sensor */
		else if (choice == 2) {
			value = read_temperature();
			display_values(value, str_value, "System Temp.: ", " C");

			if (value > 20 && value < 30) PORTD = ((1 << PIND7) | (0 << PIND4));
			else if ((value > 10 && value < 20)||(value > 30 && value < 45)){
				OCR1A = WARNING;
				PORTD &= ~((1 << PIND5) | (1 << PIND7));
				system_status(PIND6);
			}
			else {
				OCR1A = FATAL;
				PORTD &= ~((1 << PIND6) | (1 << PIND7));
				system_status(PIND5);
			}
		}
		/* Choice 3 to read from Photoresistor */
		else if (choice == 3) {
			value = read_brightness();
			display_values(value, str_value, "Brighteness: ", " %");

			if (value < 75 && value > 25) PORTD = ((1 << PIND7) | (0 << PIND4));
			else if ((value > 75 && value < 90) || (value < 25 && value > 15)){
				OCR1A = WARNING;
				PORTD &= ~((1 << PIND5) | (1 << PIND7));
				system_status(PIND6);
			}
			else {
				OCR1A = FATAL;
				PORTD &= ~((1 << PIND6) | (1 << PIND7));
				system_status(PIND5);
			}
		}
		else {
			lcd_puts("Jovan & Sons Inc.");
			lcd_putcmd(LCD_SET_CURSOR | FIRST_ROW);
		}
		
		ADCSRA |= (1 << ADSC); // Start next conversion
	}
}

