#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void ExternalInterrupt_Config(void);
void GPIO_Config(void);

int main()
{
	GPIO_Config();
	ExternalInterrupt_Config();
	
	sei();

	while(1);

	return 0;
}

void GPIO_Config()
{
	DDRB |= (1 << 5);
}

void ExternalInterrupt_Config()
{
	EICRA |= (1 << ISC01);
	EIMSK |= (1 << INT0);	
}

ISR(INT0_vect)
{
	PORTB ^= (1 << 5);
}

