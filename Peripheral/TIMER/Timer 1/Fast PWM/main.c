#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>

#define DIVIDE_1 	_BV(CS10)
#define DIVIDE_8 	_BV(CS11)
#define DIVIDE_64 	_BV(CS11)|_BV(CS10)
#define DIVIDE_256	_BV(CS12)
#define DIVIDE_1024	_BV(CS12)|_BV(CS10)

void GPIO_Config(void);

void GPIO_Config(void);
void TIMER1_Config(void);
void TIMER1_On(unsigned char prescaler);
void TIMER1_Off(void);
unsigned int TIMER1_ReadTCNT1(void);
void TIMER1_WriteTCNT1(unsigned int i);
void TIMER1_Config(void);
void TIMER1_WriteOCR1A(unsigned int i);

int main()
{
	GPIO_Config();
	TIMER1_Config();
	TIMER1_On(DIVIDE_8);
		
	sei();

	while(1)
	{
		for(uint16_t i = 0; i < 65534; i++)
		{
			TIMER1_WriteOCR1A(i);
		}

		for(uint16_t i = 65535; i > 1; i--)
		{
			TIMER1_WriteOCR1A(i);
		}
	}
	return 0;
}

ISR(TIMER1_COMPA_vect)
{
	PORTB ^= _BV(PB5);
}

void GPIO_Config()
{
	DDRB |= _BV(PB5);
}

void TIMER1_Config(void)
{
	TCCR1A |= _BV(WGM11) | _BV(WGM10);
	TCCR1B |= _BV(WGM12) | _BV(WGM13);
	TIMSK1 |= _BV(TOIE1) | _BV(OCIE1A);	
}

void TIMER1_On(unsigned char prescaler)
{
	TCCR1B |= prescaler;
}

void TIMER1_Off(void)
{
	unsigned char sreg;
	sreg = SREG;
	cli();
	TCCR1B &= ~(_BV(CS12) | _BV(CS11) | _BV(CS10));
	SREG = sreg;
}

unsigned int TIMER1_ReadTCNT1(void)
{
	unsigned char sreg;
	unsigned int i;

	sreg = SREG;
	cli();

	i = TCNT1;
	SREG = sreg;
	sei();
	return i;
}

void TIMER1_WriteTCNT1(unsigned int i)
{
	unsigned char sreg;
	sreg = SREG;
	cli();
	TCNT1 = i;
	SREG = sreg;
	sei();
}

void TIMER1_WriteOCR1A(unsigned int i)
{
	unsigned char sreg;
        sreg = SREG;
        cli();
        OCR1A = i;
        SREG = sreg;
        sei();
}
