#include <avr/io.h>
#include <util/delay.h>

void GPIO_Config(void);
void ToggleLed(void);

int main()
{
	GPIO_Config();

	while(1)
	{
		ToggleLed();
		_delay_ms(1000);
	}
}

void GPIO_Config()
{
	DDRB |= (1 << 5);
}

void ToggleLed()
{
	PORTB ^= (1 << 5);
}
