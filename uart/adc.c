#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>

#include "uartLibrary.h"

volatile char buffer1[20]="tiwari", buffer2[20]="rohan";
volatile uint8_t result1 , result2;
char ret;

void InitADC() {
	ADMUX=(1<<REFS0);               
	ADCSRA=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);    // Enabling adc enable and setting prescalars
}

uint16_t read_ADC(uint8_t pin) {
  pin=pin & 0x07;   // Selecting pin
  ADMUX &= 0xFE;    // Clearing last three bits
  ADMUX |= pin;

  ADCSRA |= (1<<ADSC);    // Selecting single conversion
  sleep_cpu();		// make cpu sleep
  return ADC;
}

ISR(ADC_vect){
  ADCSRA &= ~(1<<ADIF);  // Clearing interrupt flag
}

ISR(USART_RX_vect){
	ret  = UDR0;	// clearing buffer to make interrupt flag 0
	if(result1 < 30)
		printf("LDR ON\n");
	else
		printf("LDR OFF\n");

	if(result2 > 25 && result2 < 100)
		printf("NTC ON\n");
	else
		printf("NTC OFF\n");
}

int main(void) {
	PRR = ~(1<<PRADC | 1<<PRUSART0);   // All peripherals are disabled except adc
 	sleep_enable();    // Enabling sleep mode
	uart_init();	// Initialising usart in asynchronous mode
	InitADC();	//Initialising ADC
	sei();		// Enabling global interrupts
	DDRB |= (1<<PORTB0 | 1<<PORTB5);	// Enabling Digital Pin 13 and Digital Pin 8 in output mode
	stdout = &uart_output;
	stdin  = &uart_input;

	char input;

	while(1) {
		result1 = read_ADC(0); 
		itoa(result1,buffer1,10);
		printf("%s\t", buffer1);
		if(result1 < 30) {     // Setting value to compare for analog 
	       PORTB |= (1<<PORTB0);
	    }
	    else {
	       PORTB &= ~(1<<PORTB0);
	    }
		result2 = read_ADC(1);
		itoa(result2,buffer2,10);
		printf("%s\n", buffer2);
		if(result2 > 25 && result2 < 100){      // Setting value to compare for analog
       		PORTB |= (1<<PORTB5);
    	}
    	else {
       		PORTB &= ~(1<<PORTB5);
    	}
	}
	return 0;
}