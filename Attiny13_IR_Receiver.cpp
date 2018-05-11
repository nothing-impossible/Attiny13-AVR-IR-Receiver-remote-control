/*
 * IR Receiver.cpp
 *
 * Created: 12/1/2017 11:10:41 AM
 * Author : Dz Inventors
 */

#define F_CPU 9600000 // Must stay at this speed

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define IR_Input_Pin PORTB1

bool IR_Code(uint32_t data); // Check the IR Code
void IR_Scan(); // Scan IR data
void IR_Setup(); // Setup mode

uint32_t IR_data_out = 0; // IR data store

int main(void) {
		IR_Setup();

		DDRB |= 1 << PORTB4;
		DDRB &= ~(1 << IR_Input_Pin);
		PORTB = 0x00;

		while (1) {

				if (IR_Code(0x00FFA857)) { // Put the code you want to check it in the IR_Code function
						PORTB |= 1 << PORTB4; // Do some actions
				}
				if (IR_Code(0x00FF6897)) {
						PORTB &= ~(1 << PORTB4);
				}
				if (IR_Code(0x10EFF906)) {
						PORTB |= 1 << PORTB4;
				}
				if (IR_Code(0x10EFE916)) {
						PORTB &= ~(1 << PORTB4);
				}
		}
}
void IR_Setup() {
		GIMSK |= 1 << INT0; // Enable the interrupt pin ( at PB1)
		MCUCR = (1 << ISC00); // Set interrupt configuration in PB1 (see page 46 for any change mode)
		sei();
		TCCR0B |= 1 << CS00 | 1 << CS02; //set timer 0 with max scaler (F_CPU/1024)
}
bool IR_Code(uint32_t data) {
		// Prettified IR data output code
		uint8_t IR_data_byte[4];
		IR_data_byte[0] = (IR_data_out >> 24);
		IR_data_byte[1] = (IR_data_out >> 16);
		IR_data_byte[2] = (IR_data_out >> 8);
		IR_data_byte[3] = (IR_data_out);
		// Prettified IR data input code
		uint8_t data_byte[4];
		data_byte[0] = (data >> 24);
		data_byte[1] = (data >> 16);
		data_byte[2] = (data >> 8);
		data_byte[3] = (data);
		if (IR_data_byte[0] == data_byte[0] || IR_data_byte[1] == data_byte[1]) { // If address data is =
				if (IR_data_byte[2] == data_byte[2] || IR_data_byte[3] == data_byte[3]) { // If command data is =
						IR_data_out = 0; // Clear IR data Output
						return true;
				}
		}
		return false;
}
ISR(INT0_vect) {
		IR_Scan();
}

uint8_t time_high = 0;
uint8_t time_low = 0;

bool interrupter1 = false;
bool interrupter2 = false;
bool one_time = false;

int conter = -1;

uint8_t TCNT0_buffer = 0;
uint8_t time_span = 0;

void IR_Scan() {
		// Timer unit = 106.666 µs
		if (TCNT0 >= TCNT0_buffer) // If timer doesn't overflow
				time_span = TCNT0 - TCNT0_buffer; // Get span time
		else //if timer overflow
				time_span = (256 - TCNT0_buffer) + TCNT0; // Get span time
		if (time_span <= 2) // Delete any noise < or = then 213 µs
				return void();

		if ((PINB & (1 << IR_Input_Pin)) == 0)
				time_high = time_span; // Get the time low
		else
				time_low = time_span; // Get the time high

		if ((time_low >= 78) && (time_low <= 91)) // Low for 9ms
				interrupter1 = true; // Enable the first interrupter

		if ((time_high >= 36 && time_high <= 49) && (interrupter1 == true)) { // High time 4.5ms
				IR_data_out = 0; // Clear data
				conter = -1; // Restart counter variable
				interrupter2 = true; // Enable the second interrupter (start condition is true)
		}
		if (conter == 32) { // Check if data send is over
				interrupter1 = interrupter2 = false; // Restart interrupters
				conter = -1; // Restart conter variable
		}

		if (interrupter2 == true) { // See if start condition is true

				if (one_time == true) { // Enter one time in two loops
						if (conter != -1) { // Over enter in the first time
								if (time_high > time_low * 2) // Means we read logic 1
										IR_data_out |= 0x80000000 >> conter; // Put the logic 1 in high significant bit and shift it to the right every loop
						}
						// else
						// IR_data_out=0; // Clear data
						conter++; // Up counter
				}
				one_time = !one_time; // Reverse state
		}
		TCNT0_buffer = TCNT0; // Scan timer
}
