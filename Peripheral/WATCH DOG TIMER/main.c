#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#define tog(reg,pin) ((reg)^=(1<<(pin)))

void GPIO_Config(void);
void WDT_Config(void);

int main()
{
	GPIO_Config();
	WDT_Config();
		
	sei();

	while(1);

	return 0;
}

ISR(WDT_vect)
{
	tog(PORTB,PB5);
}

void GPIO_Config()
{
	DDRB |= (1 << PB5);
}

void WDT_Config()
{
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	WDTCSR = (1 << WDIE) | (1 << WDP3);
}

