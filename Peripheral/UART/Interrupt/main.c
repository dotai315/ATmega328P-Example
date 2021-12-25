#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>

static void UART_Config(void);
static int uart_putchar(char c, FILE* stream);

static FILE mySTD = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_RW);

static int uart_putchar(char c, FILE* stream)
{
	if(c == '\n')
	{
		uart_putchar('\r', stream);
	}
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

int main()
{
	UART_Config();
	stdout = &mySTD;

	sei();
	while(1);

	return 0;
}

ISR(USART_RX_vect)
{
	unsigned char data = UDR0;
	printf("%c\n", data);
}

static void UART_Config(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0B |= _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}
