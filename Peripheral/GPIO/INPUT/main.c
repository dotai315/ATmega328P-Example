#include <avr/io.h>
#include <stdbool.h>

void GPIO_Config(void);
bool Button_Read(void);
void Led_On(void);
void Led_Off(void);

int main()
{
	GPIO_Config();
	while(1)
	{
		if(Button_Read() != true)
		{
			Led_On();
		}
		else
		{
			Led_Off();
		}
	}
	return 0;
}

void GPIO_Config()
{
	DDRB |= (1 << 5);
	PORTB |= (1 << 4);
}

void Led_On()
{
	PORTB &= ~(1 << 5);
}

void Led_Off()
{
	PORTB |= (1 << 5);
}

bool Button_Read()
{
	if((PINB & (1 << 4)) != 0)
	{
		return false;
	}
	return true;
}
