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

static void ADC_Config(void);
static void GPIO_Config(void);
int ADC_Read(void);

int main()
{
	GPIO_Config();
	ADC_Config();
	UART_Config();
	stdout = &mySTD;

	printf("Starting ... \n");
	while(1)
	{
		printf("%d\n", ADC_Read());
		_delay_ms(1000);
	}

	return 0;
}

ISR(ADC_vect)
{
	PORTB ^= _BV(PB5);
}

static void GPIO_Config(void)
{
	DDRB |= _BV(PB5);
}

static void ADC_Config(void)
{
	ADMUX |= _BV(REFS0);
        ADCSRA |= _BV(ADEN) | _BV(ADIE);
}

static void UART_Config(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0B |= _BV(TXEN0) | _BV(RXEN0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}

int ADC_Read(void)
{
	int Ain, Alow;

	ADCSRA |= _BV(ADSC);
	loop_until_bit_is_clear(ADCSRA, ADSC);

	_delay_us(10);
	Alow = (int)ADCL;
	Ain = (int)(ADCH * 256);

	Ain = Ain + Alow;
	return Ain;
}
