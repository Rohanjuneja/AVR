#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

//initialize watchdog
void WDT_Init(void)
{
//disable interrupts
cli();
//reset watchdog
wdt_reset();
//set up WDT interrupt
WDTCSR = (1<<WDCE)|(1<<WDE);
//Start watchdog timer with 1s prescaller
WDTCSR = (1<<WDIE)|(1<<WDE)|(1<<WDP2)|(1<<WDP1);
//Enable global interrupts
sei();
}

int main()
{
	DDRB|=(1<<PORTB5);  //Set Pin As Output
	WDT_Init();  // Initialize the watchdog timer
    while(1);
}
//Watchdog timeout ISR
ISR(WDT_vect)
{
    PORTB ^= (1 << 5); // Toggle pin 13
}