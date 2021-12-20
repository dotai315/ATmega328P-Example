#include <avr/io.h>
#include <avr/interrupt.h>

#define set(reg,pin) ((reg)|=(1<<(pin)))
#define clr(reg,pin) ((reg)&=~(1<<(pin)))
#define tog(reg,pin) ((reg)^=(1<<(pin)))

void Interrupt_Config(void);
void GPIO_Config(void);

int main()
{
	GPIO_Config();
	Interrupt_Config();

	sei();

	while(1);

	return 0;
}

ISR(PCINT0_vect)
{
	cli();

	tog(PORTB,PB5);
		
	sei();
}

void GPIO_Config()
{
	set(DDRB,PB5);
}

void Interrupt_Config()
{
	set(PCICR,PCIE0);
	set(PCMSK0,PCINT4);
}
