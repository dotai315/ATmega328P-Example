#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DIVIDE_1 	_BV(CS10)
#define DIVIDE_8 	_BV(CS11)
#define DIVIDE_64 	_BV(CS11)|_BV(CS10)
#define DIVIDE_256	_BV(CS12)
#define DIVIDE_1024	_BV(CS12)|_BV(CS10)

void GPIO_Config(void);
void TIMER1_Config(void);
void TIMER1_On(unsigned int prescaler);
void TIMER1_SetOCR(unsigned int value);

int main(void)
{
	GPIO_Config();
	TIMER1_Config();
	TIMER1_On(DIVIDE_8);
	
	sei();

	while(1)
	{
		for(uint16_t i = 0; i < 254; i++)
		{
			TIMER1_SetOCR(i);
			_delay_ms(8);
		}
		for(uint16_t i = 254; i > 1; i--)
		{
			TIMER1_SetOCR(i);
			_delay_ms(8);
		}
	}
	return 0;
}

ISR(TIMER1_COMPA_vect)
{
	PORTB ^= _BV(PB5);
}

void GPIO_Config(void)
{
	DDRB |= _BV(PB5);
}

void TIMER1_Config(void)
{
	TCCR1A |= _BV(WGM11) | _BV(WGM10);
	TCCR1B |= _BV(WGM13);
	TIMSK1 |= _BV(OCIE1A);
}

void TIMER1_On(unsigned int prescaler)
{
	TCCR1B |= prescaler;
}

void TIMER1_SetOCR(unsigned int value)
{
	OCR1A = value;
}
