#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <stdbool.h>

#define DIVIDE_1 	_BV(CS10)
#define DIVIDE_8 	_BV(CS11)
#define DIVIDE_64 	_BV(CS11)|_BV(CS10)
#define DIVIDE_256	_BV(CS12)
#define DIVIDE_1024	_BV(CS12)|_BV(CS10)

volatile bool loop;

void GPIO_Config(void);
void LED_On(void);
void LED_Off(void);
void TIMER1_Delay_Ms(unsigned int period);
void TIMER1_Config(void);
void TIMER1_On(unsigned char prescaler);
void TIMER1_Off(void);
unsigned int TIMER1_ReadTCNT1(void);
void TIMER1_WriteTCNT1(unsigned int i);
void TIMER1_Delay_1Ms(void);
void TIMER1_Config(void);
void TIMER1_WriteOCR1A(unsigned int i);

int main()
{
	GPIO_Config();
	TIMER1_Config();
	
	sei();
	
	TIMER1_WriteOCR1A(0xF424);	
	TIMER1_On(DIVIDE_256);
	
	while(1);
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
	TCCR1B |= _BV(WGM12);
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
