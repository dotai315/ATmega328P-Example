#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#include <util/setbaud.h>

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

uint8_t writeNum = 123;
uint8_t readNum;

static void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
static unsigned char EEPROM_read(unsigned int uiAddress);

int main()
{
	UART_Config();
	stdout = &mySTD;

	EEPROM_write(96,writeNum);
	readNum = EEPROM_read(96);

	printf("num = %d\n", readNum);
	while(1);
	return 0;
}

static void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	while(EECR & (1<<EEPE));
	EEAR = uiAddress;
	EEDR = ucData;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}

static unsigned char EEPROM_read(unsigned int uiAddress)
{
	while(EECR & (1<<EEPE));
	EEAR = uiAddress;
	EECR |= (1<<EERE);
	return EEDR;
}


static void UART_Config(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0B |= _BV(TXEN0) | _BV(RXEN0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}
