#include<avr/io.h>
#include<util/delay.h>
#define F_CPU 16000000UL
#include <avr/interrupt.h>
# include <avr/sleep.h>
// here we will use adc in AUTO TRIGGER MODE!!!
ISR (ADC_vect){
        
  
  TIFR0=(1<<OCF0A);//clearing the interrupt flag to allow future interrupts
  if((ADMUX & 0x01)==0){// checking if the analogue channel is A0
  if(ADC > 80 ){
  PORTB |= (1<<5);
  }
  else{
  PORTB &= ~(1<<5);
  }
   ADMUX |=(0x01);//setting channel to A1
  }
  else{

  if(ADC > 120 ){
  
  PORTB |= (1<<0);
  
  }
  else{
  PORTB &= ~(1<<0);
  
  
  }
  ADMUX &= 0xFE;// switching the channel

  }
      //  ADCSRA |= (1<<6);//starting the conversion
}


void timer_setup(){
  TCNT0=0;// setting the counter to 0
  OCR0A=100;// setting top value of counter to 100( found by calculation)
  TCCR0A |=(1<<COM0A0)|(1<<WGM01);// setting the ctc mode
  TCCR0B |=(1<<CS01);// setting the prescalar
  }


void main ()
{
 ADMUX |= (1<<6);//setting refs0 bit to allow avcc as voltage reference
 ADCSRA |= (1<<7) | (1<<5) | (1<<3) | (1<<2) | (1<<1) | (1<<0); //setting adc enable and selecting prescalars to 128 khz along with ADATE bit!!
 DDRB |= (1<<0) | (1<<5);//setting PORTB5 and PORTB6 as output 
 timer_setup();//setting up the timer
 set_sleep_mode(SLEEP_MODE_ADC);// setting ADC noise reduction mode
 sleep_enable();
 sei();
 //sleep_cpu();
 //sleep_disable();
// ADCSRA |= (1<<6);//starting the conversion
 ADMUX &= 0xF0;//setting channel to A0
 ADMUX |= (1<<0);//setting channel to A1
 ADCSRB |= (1<<ADTS1)|(1<<ADTS0);// setting Timer0 as trigger source
 for(;;){ // infinite loop
  }



}