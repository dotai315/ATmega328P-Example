#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#include <util/setbaud.h>
#include <avr/power.h>

static void UART_Config(void);
static int uart_putchar(char c, FILE* stream);

static FILE mySTD = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_RW);

static int uart_putchar(char c, FILE* stream)
{
	if(c == '\n')
	{
		uart_putchar('\r', stream);
	}
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

void printByte(uint8_t byte)
{
        /* Converts a byte to a string of decimal text, sends it */
        printf("%d",'0'+ (byte/100));
        /* Hundreds */
        printf("%d",'0'+ ((byte/10) % 10)); 
        /* Tens*/
        printf("%d",'0'+ (byte % 10));
        /* Ones*/
}


void initI2C(void);
/* Sets pullups and initializes bus speed to 100kHz (at FCPU=8MHz) */
void i2cWaitForComplete(void);
/* Waits until the hardware sets the TWINT flag */
void i2cStart(void);
/* Sends a start condition (sets TWSTA) */
void i2cStop(void);
/* Sends a stop condition (sets TWSTO) */
void i2cSend(uint8_t data);
/* Loads data, sends it out, waiting for completion */
uint8_t i2cReadAck(void);
/* Read in from slave, sending ACK when done (sets TWEA) */
uint8_t i2cReadNoAck(void);
/* Read in from slave, sending NOACK when done (no TWEA) */

// -------- Defines -------- //
#define LM75_ADDRESS_W		0b10010000
#define LM75_ADDRESS_R		0b10010001
#define LM75_TEMP_REGISTER	0b00000000
#define LM75_CONFIG_REGISTER	0b00000001
#define LM75_THYST_REGISTER	0b00000010
#define LM75_TOS_REGISTER	0b00000011
// -------- Functions --------- //

int main(void) 
{
	uint8_t tempHighByte, tempLowByte;
	// -------- Inits --------- //
	clock_prescale_set(clock_div_1);
	UART_Config();
	stdout = &mySTD;
	printf("\r\n==== i2c Thermometer ====\r\n");
	initI2C();

	// ------ Event loop ------ //
	while (1) 
	{
		/* To set register, address LM75 in write mode */
		i2cStart();
		i2cSend(LM75_ADDRESS_W);
		i2cSend(LM75_TEMP_REGISTER);
		i2cStart();
		/* restart, just send start again */
		/* Setup and send address, with read bit */
		i2cSend(LM75_ADDRESS_R);
		/* Now receive two bytes of temperature */
		tempHighByte = i2cReadAck();
		tempLowByte = i2cReadNoAck();
		i2cStop();
		// Print it out nicely over serial for now...
		printByte(tempHighByte);
		if (tempLowByte & _BV(7)) 
		{
			printf(".5\r\n");
		}
		else 
		{
			printf(".0\r\n");
		}
		/* Once per second */
		_delay_ms(1000);
	}
	return (0);
	/* End event loop */
	/* This line is never reached */
}

void initI2C(void)
{
	TWBR = 32;
	TWCR |= (1 << TWEN);
	/* set bit rate, see p. 242 */
	/* 8MHz / (16+2*TWBR*1) ~= 100kHz */
	/* enable */
}
void i2cWaitForComplete(void) 
{
	loop_until_bit_is_set(TWCR, TWINT);
}

void i2cStart(void) 
{
	TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTA));
	i2cWaitForComplete();
}

void i2cStop(void) 
{
	TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTO));
}

uint8_t i2cReadAck(void) 
{
	TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWEA));
	i2cWaitForComplete();
	return (TWDR);
}

uint8_t i2cReadNoAck(void) 
{
	TWCR = (_BV(TWINT) | _BV(TWEN));
	i2cWaitForComplete();
	return (TWDR);
}

void i2cSend(uint8_t data) 
{
	TWDR = data;
	TWCR = (_BV(TWINT) | _BV(TWEN));
	i2cWaitForComplete();
}

static void UART_Config(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0B |= _BV(TXEN0) | _BV(RXEN0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}

