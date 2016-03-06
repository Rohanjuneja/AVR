/*
 * Demonstration on how to redirect stdio to UART. 
 */

#include <stdio.h>
#include <util/delay.h>

#include "uartLibrary.h"

int main(void) {

	uart_init();
	stdout = &uart_output;
	stdin  = &uart_input;

	char input;
	int i;

	while(1) {
		puts("Hello world!");
		input = getchar();
		printf("You wrote %c\n", input);        
	}

	return 0;
}