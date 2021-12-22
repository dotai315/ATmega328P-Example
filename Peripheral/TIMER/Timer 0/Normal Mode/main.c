#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

volatile unsigned int cnt;

void GPIO_Config(void);
void LED_On(void);
void LED_Off(void);
void TIMER0_Delay_Ms(unsigned int period);
void TIMER0_Config(void);
void TIMER0_On(void);
void TIMER0_OFf(void);

int main()
{
	GPIO_Config();
	TIMER0_Config();
	
	sei();

	while(1)
	{
		LED_On();
		TIMER0_Delay_Ms(1000);
		LED_Off();
		TIMER0_Delay_Ms(1000);
	}
	return 0;
}

ISR(TIMER0_OVF_vect)
{
	cnt--;
	TCNT0 = 0xEF;
}

void GPIO_Config()
{
	DDRB |= _BV(PB5); 
}

void LED_On(void)
{
	PORTB |= _BV(PB5);	
}
void LED_Off(void)
{
	PORTB &= ~(_BV(PB5));
}

void TIMER0_Config()
{
	TCNT0 = 0xEF;
	TIMSK0 |= _BV(TOIE0);	
}

void TIMER2_On()
{
	TCCR0B |= _BV(CS02) | _BV(CS01) | _BV(CS00);
}

void TIMER0_Off()
{
	TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00));
}

void TIMER0_Delay_Ms(unsigned int period)
{
	cnt = period;
	TIMER0_On();
	while(cnt);
	TIMER0_Off();
}

