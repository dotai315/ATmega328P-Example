#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#include <util/setbaud.h>

static void UART_Config(void);
static int uart_putchar(char c, FILE* stream);
static int uart_getchar(FILE* stream);

static FILE mySTD = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

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

static int uart_getchar(FILE* stream)
{
	uint8_t c;
	char* cp, * cp2;
	static char b[80];
	static char *rxp;

	if(rxp == 0)
	{
		for(cp = b;;)
		{
			loop_until_bit_is_set(UCSR0A, RXC0);
			if(UCSR0A & _BV(FE0))
			{
				return _FDEV_EOF;
			}
			if(UCSR0A & _BV(DOR0))
			{
				return _FDEV_ERR;
			}
			c = UDR0;

			if(c == '\r')
			{
				c = '\n';
			}
			if(c == '\n')
			{
				*cp = c;
				uart_putchar(c, stream);
				rxp = b;
				break;
			}
			else if(c == '\t')
			{
				c = ' ';
			}

			if((c >= (uint8_t)' ' && c <= (uint8_t)'\x7e') || c >= (uint8_t)'\xa0')
			{
				if(cp == b + 80 - 1)
				{
					uart_putchar('\a', stream);
				}
				else
				{
					*cp++ = c;
					uart_putchar(c, stream);
				}
				continue;
			}

			switch(c)
			{
			case 'c' & 0x1f:
				return -1;
			case '\b':
			case '\x7f':
				if(cp > b)
				{
					uart_putchar('\b', stream);
					uart_putchar(' ', stream);
					uart_putchar('\b', stream);
					cp--;
				}
				break;
			case 'r' & 0x1f:
				uart_putchar('\r', stream);
				for(cp2 = b; cp2 < cp; cp2++)
				{
					uart_putchar(*cp2, stream);
				}
				break;
			case 'u' & 0x1f:
				while(cp > b)
				{
					uart_putchar('\b', stream);
					uart_putchar(' ', stream);
					uart_putchar('\b', stream);
					cp--;
				}
				break;
			case 'w' & 0x1f:
				while(cp > b && cp[-1] != ' ')
				{
					uart_putchar('\b', stream);
					uart_putchar(' ', stream);
					uart_putchar('\b', stream);
					cp--;
				}
				break;
			}
		}
	}

	c = *rxp++;
	if(c == '\n')
	{
		rxp = 0;
	}
	return c;
}


int main()
{
	UART_Config();
	stdout = &mySTD;
	stdin = &mySTD;
	stderr = &mySTD;
	
	int num = 0;

	fprintf(stderr, "Hello world!\n");

	while(1)
	{
		printf("Hello World!\n");
		scanf("%d", &num);
		printf("num: %d\n", num);
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
