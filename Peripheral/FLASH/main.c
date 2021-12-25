#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#include <util/setbaud.h>
#include <avr/pgmspace.h>

const char myVeryLongString[] PROGMEM = "\r\nHi there, \
this is an example of a long string.\r\n\
The kind that you wouldn't want to store in RAM.\r\n";

const uint16_t sixteenBits PROGMEM = 12345;

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

int main(void)
{
	UART_Config();
	stdout = &mySTD;
	
	char oneLetter;
	uint8_t i;

	while(1)
	{
		for (i = 0; i < sizeof(myVeryLongString); i++) 
		{
			oneLetter = pgm_read_byte(&(myVeryLongString[i]));
			printf("%c\n",oneLetter);
			_delay_ms(100);
		}
		_delay_ms(1000);

		printf("%d\n", sixteenBits);
	}
	return 0;
}

static void UART_Config(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0B |= _BV(TXEN0) | _BV(RXEN0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}
