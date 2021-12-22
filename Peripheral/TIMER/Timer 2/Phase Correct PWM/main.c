#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>

void GPIO_Config(void);
void TIMER2_Config(void);
void TIMER2_SetOCR2A(unsigned char value);
void TIMER2_On(void);
void TIMER2_Off(void);

int main()
{
	GPIO_Config();
	TIMER2_Config();
	TIMER2_On();

	sei();

	while(1)
	{
		for(uint8_t i = 0; i < 254; i++)
		{
			TIMER2_SetOCR2A(i);
			_delay_ms(8);
		}

		for(uint8_t i = 255; i > 1; i--)
		{
			TIMER2_SetOCR2A(i);
			_delay_ms(8);
		}
	}
	return 0;
}

ISR(TIMER2_COMPA_vect)
{
	PORTB ^= _BV(PB5);
}

void GPIO_Config()
{
	DDRB |= _BV(PB5);
}

void TIMER2_Config()
{
	TCCR2A |= _BV(WGM20);
	TCCR2B |= _BV(WGM22);
	TIMSK2 |= _BV(OCIE2A);
}

void TIMER2_On()
{
	TCCR2B |= _BV(CS21);
}

void TIMER2_Off()
{
	TCCR2B &= ~(_BV(CS21));
}

void TIMER2_SetOCR2A(unsigned char value)
{
	OCR2A = value;
}

