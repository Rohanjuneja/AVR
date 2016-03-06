#include <avr/io.h>
#include <util/delay.h>

int main(void){
   DDRB |= 0b00100000; // Setting 13th pin as output
   PORTB = 0b00100000; // Setting 13th pin to high

   while(1){
       PORTB ^= 1 << PINB5; // using Xor to toggle pin value
       _delay_ms(100);
   	}
   	return 0;
}