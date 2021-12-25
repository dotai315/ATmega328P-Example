#define BAUD 9600

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <stdio.h>
#include <util/setbaud.h>

#define MASTER 1

static void GPIO_Config(void);
static void SPI_Config(void);

static void UART_Config(void);
static int uart_putchar(char c, FILE* stream);

static FILE mySTD = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

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

#if MASTER
static void SPI_Transmit(char cData)
{
	SPDR = cData;
	loop_until_bit_is_set(SPSR,SPIF);
}
#else
static char SPI_Receive(void)
{
	loop_until_bit_is_set(SPSR,SPIF);
	return SPDR;
}
#endif

int main()
{
	GPIO_Config();
	SPI_Config();
	UART_Config();
	stdout = &mySTD;

#if MASTER
	SPI_Transmit('A');
#else
	printf("%c\n",SPI_Receive());
#endif
	while(1);
	return 0;
}

static void GPIO_Config(void)
{
#if MASTER
	DDRB |= _BV(PB5) | _BV(PB3) | _BV(PB2);
#else
	DDRB |= _BV(PB4);
#endif
}

static void SPI_Config(void)
{
#if MASTER
	SPCR |=  _BV(SPE) | _BV(MSTR) | _BV(SPR0);
#else 
	SPCR |= _BV(SPE);
#endif
}
static void UART_Config(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0B |= _BV(TXEN0) | _BV(RXEN0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}
