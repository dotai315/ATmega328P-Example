#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>

#define DIVIDE_1 	_BV(CS10)
#define DIVIDE_8 	_BV(CS11)
#define DIVIDE_64 	_BV(CS11)|_BV(CS10)
#define DIVIDE_256	_BV(CS12)
#define DIVIDE_1024	_BV(CS12)|_BV(CS10)

void GPIO_Config(void);
void TIMER1_Config(void);
void TIMER1_On(unsigned int prescaler);

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
	TIMER1_Config();
	TIMER1_On(DIVIDE_1);
	UART_Config();
	stdout = &mySTD;
	
	sei();

	while(1);
	return 0;
}

ISR(TIMER1_CAPT_vect)
{
	uint16_t num = ICR1;
	printf("\nICR1: %d\n", num);
}

void TIMER1_Config(void)
{
	TCCR1B |=  _BV(ICNC1);
	TIMSK1 |= _BV(ICIE1);
}

void TIMER1_On(unsigned int prescaler)
{
	TCCR1B |= prescaler;
}

static void UART_Config(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0B |= _BV(TXEN0) | _BV(RXEN0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}

