#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

void GPIO_Config(void);
void TIMER0_Config(void);
void TIMER0_On(void);
void TIMER0_Off(void);
void TIMER0_SetOCR0A(unsigned char value);

int main()
{
	GPIO_Config();
	TIMER0_Config();

	sei();

	while(1)
	{
		for(uint8_t i = 0; i < 255; i++)
		{
			TIMER0_SetOCR0A(i);
			_delay_ms(8);
		}
		for(uint8_t i = 255; i > 1; i--)
		{
			TIMER0_SetOCR0A(i);
			_delay_ms(8);
		}
	}
	return 0;
}

ISR(TIMER0_COMPA_vect)
{
	PORTB ^= _BV(PB5);
}

void GPIO_Config()
{
	DDRB |=  _BV(PB5);
}

void TIMER0_Config()
{
	TCCR0A |= _BV(WGM01);
	TIMSK0 |= _BV(OCIE0A);
	TCCR0B |= _BV(CS01);
}

void TIMER0_SetOCR0A(unsigned char value)
{
	OCR0A = value;
}
