#include <avr/io.h>
#include <util/delay.h>

#define E_Port  PORTB                   // lcd Enable pin
#define RS_Port PORTB                   // lcd Register Select pin

// LCD instructions
#define lcd_Clear           0b00000001          // clear the display
#define lcd_Display       0b00001100          // display on
#define lcd_4bit 0b00101000          // 4-bit data, 2-line display, 5 x 7 font

// Program ID
uint8_t name1[] = "Rohan Tiwari";
uint8_t name2[] = "Rohan Juneja";

void init(void)
{
// Power-up delay
    _delay_ms(100); 

// Set up the RS and E lines for the 'write_data' subroutine.
    RS_Port &= ~(1<<PORTB0);                  // select the Instruction Register (RS low)
    E_Port &= ~(1<<PORTB1);                   // make sure E is initially low

// Function Set instruction
    write_instruction(lcd_4bit);   			  // set mode
    _delay_us(80);                            

 
// Display On/Off Control instruction
    write_instruction(lcd_Display);           // turn the display ON
    _delay_us(80);                            
}

void write_string(uint8_t string[])
{
    volatile int i = 0;                       // character counter
    while (string[i] != 0)
    {
        write_character(string[i]);
        i++;
        _delay_us(80);                       
    }
}
lcd_Display);      
void write_character(uint8_t data)
{
    RS_Port |= (1<<PORTB0);                  // select the Data Register (RS high)
    E_Port &= ~(1<<PORTB1);                  // make sure E is initially low
    write_data(data);                        // write the upper 4-bits of the data
    write_data(data << 4);                   // write the lower 4-bits of the data
}
void write_instruction(uint8_t instruction)
{
    RS_Port &= ~(1<<PORTB0);                 // select the Instruction Register (RS low)
    E_Port &= ~(1<<PORTB1);                  // make sure E is initially low
    write_data(instruction);                 // write the upper 4-bits of the data
    write_data(instruction << 4);            // write the lower 4-bits of the data
}

void write_data(uint8_t byte)
{
    PORTD &= ~(1<<PORTD7);                        // assume that data is '0'
    if (byte & 1<<7) PORTD |= (1<<PORTD7);     // make data = '1' if necessary

    PORTD &= ~(1<<PORTD6);                        // repeat for each data bit
    if (byte & 1<<6) PORTD |= (1<<PORTD6);

    PORTD &= ~(1<<PORTD5);
    if (byte & 1<<5) PORTD |= (1<<PORTD5);

    PORTD &= ~(1<<PORTD4);
    if (byte & 1<<4) PORTD |= (1<<PORTD4);

// write the data
    E_Port |= (1<<PORTB1);      // Enable pin high
    _delay_us(1);
    E_Port &= ~(1<<PORTB1);     // Enable pin low
    _delay_us(1);
}


int main()
{
// configure the microprocessor pins for the data lines
    DDRD |= (1<<PORTD4);
    DDRD |= (1<<PORTD5);   
    DDRD |= (1<<PORTD6);            // 4 data lines : output
    DDRD |= (1<<PORTD7);

// configure the microprocessor pins for the control lines
    DDRB |= (1<<PORTB0);     // RS line : output
    DDRB |= (1<<PORTB1);     // E line : output

// initialize the LCD controller as determined by the defines (LCD instructions)
    init();             // initialize the LCD display for a 4-bit interface

// endless loop
    while(1){
        write_string(name1);                      // writing first name
        _delay_ms(2000);
        write_instruction(lcd_Clear);             // clear display RAM
        _delay_ms(4);                             
        write_string(name2);                      // writing second name
        _delay_ms(2000);
        write_instruction(lcd_Clear);             // clear display RAM
        _delay_ms(4);

    }
    return 0;
}