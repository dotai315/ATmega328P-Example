#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

volatile unsigned int cnt;

void GPIO_Config(void);
void LED_On(void);
void LED_Off(void);
void TIMER2_Delay_Ms(unsigned int period);
void TIMER2_Config(void);
void TIMER2_On(void);
void TIMER2_OFf(void);

int main()
{
	GPIO_Config();
	TIMER2_Config();
	
	sei();

	while(1)
	{
		LED_On();
		TIMER2_Delay_Ms(1000);
		LED_Off();
		TIMER2_Delay_Ms(1000);
	}
	return 0;
}

ISR(TIMER2_OVF_vect)
{
	cnt--;
	TCNT2 = 0xEF;
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

void TIMER2_Config()
{
	TCNT2 = 0xEF;
	TIMSK2 |= _BV(TOIE2);	
}

void TIMER2_On()
{
	TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);
}

void TIMER2_Off()
{
	TCCR2B &= ~(_BV(CS22) | _BV(CS21) | _BV(CS20));
}

void TIMER2_Delay_Ms(unsigned int period)
{
	cnt = period;
	TIMER2_On();
	while(cnt);
	TIMER2_Off();
}

