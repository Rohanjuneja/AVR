#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/wdt.h>

#define Clear_LCD 0b00000001
#define Set_4BitMode 0b00101000
#define DisplayOn_CursorOff 0b00001100
#define Line_Two 0b11000000
#define Line_One 0b10000000

uint8_t name1 = 1;
uint8_t name2 = 2;
volatile uint8_t flag = 0;
volatile uint8_t flag2 = 0;
volatile uint8_t flag3 = 0;
volatile int count = 0;
volatile int count1 = 0;
volatile uint8_t portbhistory;

char buffer[20];
    
void write_data(uint8_t data)
{
	PORTB |= (1<<PORTB0); // Setting RS to 1
	PORTB &= ~(1<<PORTB1); // Setting E to 0
    _delay_us(1);
	PORTD &= 0x0F;
    PORTD |=  (data & 0xF0);
    PORTB |= (1<<PORTB1);      // Enable pin high
    _delay_us(1);
    PORTB &= ~(1<<PORTB1);      // Enable pin high
    _delay_us(1);
    data = (data << 4);

    PORTB &= ~(1<<PORTB1); // Setting E to 0
    _delay_us(1);
    PORTD &= 0x0F;
    PORTD |=  (data & 0xF0);
    PORTB |= (1<<PORTB1);      // Enable pin high
    _delay_us(1);
    PORTB &= ~(1<<PORTB1);      // Enable pin high
    _delay_us(1);   
}
void write_instruction(uint8_t data)
{
    PORTB &= ~(1<<PORTB0); // Setting RS to 0
    PORTB &= ~(1<<PORTB1); // Setting E to 0
    PORTD &= 0x0F;
    PORTD |=  (data & 0xF0);
    PORTB |= (1<<PORTB1);      // Enable pin high
    _delay_us(1);
    PORTB&= ~(1<<PORTB1);     // Enable pin low
    _delay_us(1);
    data = (data << 4);
    PORTB &= ~(1<<PORTB0); // Setting RS to 1
    PORTB &= ~(1<<PORTB1); // Setting E to 0
    PORTD &= 0x0F;
    PORTD |=  (data & 0xF0);
    PORTB |= (1<<PORTB1);      // Enable pin high
    _delay_us(1);
    PORTB &= ~(1<<PORTB1);     // Enable pin low
    _delay_us(1);   
}
    
void write_string(uint8_t string[])
{
    int i = 0;                       // character counter
    while (string[i] != 0)
    {
        write_data(string[i++]);
        _delay_us(80);                 
    }
}
void delay_200()
{
    TCNT1 = 0; // Reseting the timer
    while ( TCNT1 <= 49999); // Setting 200 milisec Timer
}
int main()
{
    _delay_ms(1000);
	// configure the pins for the data lines

	DDRD |= 0xF0;

    // configure the  pins for the control lines
    DDRB |= (1<<PORTB0);     // RS line : outputx
    DDRB |= (1<<PORTB1);     // E line : output
    DDRB |= (1<<PORTB5);    // Settigng the LED 
    DDRB |= (1<<PORTB4);    // Settigng the LED 

    EICRA = (1<<ISC01);    // set INT0 to trigger on rising edge
    EIMSK |= (1 << INT1 | 1 << INT0);     // Turns on INT0

    PCICR |= (1 << PCIE1 | 1<< PCIE0);    // set PCIE0 to enable PCMSK0 scan
    PCMSK1 |= (1 << PCINT8);  // set PCINT0 to trigger an interrupt on state change 
    PCMSK0 |= (1 << PCINT2);
    TCCR1B |= ((1 << CS10 ) | (1 << CS11 ) ) ; // Set up timer at Fcpu /64




    write_instruction(DisplayOn_CursorOff);
    _delay_us(80);
    write_instruction(Clear_LCD);
    _delay_us(80);
    write_instruction(Set_4BitMode);
    _delay_us(80);
    // WDT_Init();  // Initialize the watchdog timer
    write_instruction(Line_Two);
    _delay_ms(4);
    itoa(count1,buffer,10);
    write_string(buffer);
    _delay_ms(4);
    write_instruction(Line_Two);
    _delay_ms(4);
    itoa(count1,buffer,10);
    write_string(buffer);
    _delay_ms(4);
    sei();
    while(1)
    {
        if(flag2)
        {
    
            if(flag3)
            {
                write_instruction(Line_Two); // Set Line 2
                _delay_ms(4);
                itoa(count1,buffer,10); // Converting integer to string
                write_string(buffer);
                delay_200();
                flag3 = 0;
                flag2 = 0;
                flag = 0;
            } 
            else {
                write_instruction(Line_One); // Set Line 1
                _delay_ms(4); //Delay Used By LCD
                itoa(count,buffer,10); // Converting integer to string
                write_string(buffer);
                _delay_ms(4); // Delay Used By LCD
                delay_200();
                flag2 = 0;
                flag = 0;
                flag3 = 0;
            }
        }

    }
    return 0;
}

ISR(INT1_vect)
{
    if(!flag) {
        flag = 1;
        flag2 = 1;
        flag3 = 0;
        count++; // Incrementing the number on LCD
    }
}
ISR(INT0_vect)
{
    if(!flag) {
        flag = 1;
        flag3 = 1;
        flag2 = 1;
        count1--; // Decrementing the number on LCD
    }
}
ISR(PCINT1_vect)
{
    if(!flag)
    {
        PORTB  ^= (1<<PORTB5); // Toggle the LED
        flag2 = 1;
        flag = 1;
    }
}
ISR(PCINT0_vect)
{
    if(!flag)
    {
        PORTB  ^= (1<<PORTB4); // Toggle the LED
        flag2 = 1;
        flag = 1;
    }
}