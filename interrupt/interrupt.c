#include <avr/io.h>
#include <avr/interrupt.h>

int main() {
	DDRB |= 0x20;	// Set led as output
	TCCR1B |= ((1 << WGM12));	// Setting timer 1 at CTC mode 
	TCCR1B |= ((1 << CS10) | (1 << CS12));	// Setting timer at Fcpu/1024
	TIMSK1 |= (1 << OCIE1A);	// Enable CTC output/compare interrupt
	sei();	// Enable global interrupts
	OCR1A = 15624;	// Set CTC compare value at 1Hz, with prescalar 1024
		// Number of ticks = Input frquency / (prescalar * Target frequency) - 1
		// 16000000/(1024*1) - 1 = 15624

	while(1) {
	}
	return 0;
}

ISR( TIMER1_COMPA_vect ) {
	PORTB ^= (1 << PORTB5); 	// Toggling the led
}