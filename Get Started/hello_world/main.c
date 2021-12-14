#ifndef F_CPU
#define F_CPU 16000000UL
#endif 

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>

void UART_Init(unsigned int ubrr);
void UART_sendString(const char * str);
void UART_Transmit(unsigned char data);

int main()
{
	UART_Init(MYUBRR);
	
	sei();

	while(1)
	{
		UART_sendString("Hello World\n\r");
		_delay_ms(1000);
	}

	return 0;
}



void UART_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B |= (1 << TXEN0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_Transmit(unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void UART_sendString(const char* str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		UART_Transmit(str[i]);
		i++;
	}
}


