#ifndef TWI_INTERRUPT_H
#define TWI_INTERRUPT_H

#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>


volatile uint8_t my_twi_address;
volatile uint8_t next_byte_to_transmit;

uint8_t buffer_size;
uint8_t *buffer;


void TWISlaveInit(uint8_t address, uint8_t mask__);
void TWISetBuffer(uint8_t *buffer_, uint8_t buffer_size_);
void TWIDisable();
uint8_t TWIGetStatus();

//ISR(TWI_vect);


#endif // TWI_INTERRUPT_H