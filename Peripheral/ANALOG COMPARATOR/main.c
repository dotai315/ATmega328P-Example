#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

static void GPIO_Config(void);
static void AnalogComparator_Config(void);

int main(void)
{
	GPIO_Config();
	AnalogComparator_Config();
	while(1);
	return 0;
}

ISR(ANALOG_COMP_vect)
{
	if(bit_is_set(ACSR,ACO))
	{
		PORTB &= ~(_BV(PB5));
	}
	else
	{
		PORTB |= _BV(PB5);
	}
}

static void GPIO_Config(void)
{
	DDRB |= _BV(PB5);
}

static void AnalogComparator_Config(void)
{
	cli();
	ACSR |= _BV(ACIE);
	sei();
}
