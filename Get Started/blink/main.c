#include <avr/io.h>
#include <util/delay.h>

void Led_Config();
void Led_On();
void Led_Off();

void main()
{
	Led_Config();
	while(1)
	{
		Led_On();
		_delay_ms(500);
		Led_Off();
		_delay_ms(500);
	}
}

void Led_Config()
{
	DDRB |= (1 << 5);
}

void Led_On()
{
	PORTB |= (1 << 5);
}

void Led_Off()
{
	PORTB &= ~(1 << 5);
}

