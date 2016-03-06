#include <stdio.h>
#define SFR(addr) (*(volatile uint8_t *)((addr) + 0x20))	// sfr makes special function registers look like C variables.
#define DDRB SFR(0x04)
#define PORTB SFR(0x05) 

void delay(unsigned long delay) {		// Wasting CPU cycles to implement delay function
  	volatile unsigned long i = 0, a = 750; 
  	while(a--) {
  		i = 0;
  		for (i = 0; i < delay/2; i++) {
  			__asm__ __volatile__ ("nop");	// nop statement, assembly language
  										// executes in one machine cycle, giving delay of approx 62.5 ns delay
 		}
 	}
}

int main() {
	DDRB |= 0x20;	// Setting 5th bit as output without disturbing the other pins
	while(1){
       	PORTB ^= (1 << 5);	//Toggling the 5th bit of PORTB
       	delay(1000);
   	}
	return 0;
}