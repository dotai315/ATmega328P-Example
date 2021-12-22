#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>

void GPIO_Config(void);
void TIMER0_Config(void);
void TIMER0_SetOCR0A(unsigned char value);
void TIMER0_On(void);
void TIMER0_Off(void);

int main()
{
	GPIO_Config();
	TIMER0_Config();
	TIMER0_On();

	sei();

	while(1)
	{
		for(uint8_t i = 0; i < 254; i++)
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
	DDRB |= _BV(PB5);
}

void TIMER0_Config()
{
	TCCR0A |= _BV(WGM00);
	TCCR0B |= _BV(WGM02);
	TIMSK0 |= _BV(OCIE0A);
}

void TIMER0_On()
{
	TCCR0B |= _BV(CS01);
}

void TIMER0_Off()
{
	TCCR0B &= ~(_BV(CS01));
}

void TIMER0_SetOCR0A(unsigned char value)
{
	OCR0A = value;
}

