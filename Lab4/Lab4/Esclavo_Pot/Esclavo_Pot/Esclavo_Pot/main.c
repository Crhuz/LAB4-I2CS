
#define F_CPU 16000000 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ADC.h"
#include "I2C.h"


#define SlaveAddress 0x30

uint8_t buffer = 0;
uint16_t valueADC = 0;

int main(void)
{
	I2C_Slave_Init (SlaveAddress);
	ADC_init();
	
	sei();
	
    while (1) 
    {
		valueADC = ADC_read(0) >> 2;		
    }
}

ISR(TWI_vect){
	
	uint8_t estado;
	estado = TWSR & 0xFC;
	switch (estado){
		case  0x60:
		case 0x70:
			TWCR |= (1<<TWINT);
			break;
		case 0x80:
		case 0x90:
			buffer = TWDR;
			TWCR |= (1 << TWINT); //Bandera limpia			
			break;
		case 0xA8:
		case 0xB8:
			TWDR = valueADC;	
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWEA); //Envia
 			break;
		default:		
			TWCR |= (1 << TWINT) | (1 << TWSTO); //Libera errores
			break;
	}
	
}

