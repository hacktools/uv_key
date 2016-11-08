#include "twi_interrupt.h"


void TWIMasterInit() { 
    //set SCL to 100kHz (for 8MHz clock)
    TWSR0 = 0x00;
    TWBR0 = 0x20;
    //enable TWI
    TWCR0 = (1<<TWEN) | (1<<TWINT);
}

void TWISlaveInit(uint8_t address, uint8_t mask__) {
    //set SCL to 100kHz (for 8MHz clock)
    TWSR0 = 0x00;
    TWBR0 = 0x20;
    // load address into TWI address register
    TWAR0 = (address << 1);
    TWAMR0 = mask__;

    // set the TWCR to enable address matching and enable TWI, clear TWINT, enable TWI interrupt
    TWCR0 = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
}

void TWIMasterStart() {
    TWCR0 = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while ((TWCR0 & (1<<TWINT)) == 0);
}

void TWIMasterStop() {
    TWCR0 = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void TWIDisable() {
    // clear acknowledge and enable bits
    TWCR0 &= ~( (1<<TWEA) | (1<<TWEN) );
}

void TWIMasterWrite(uint8_t u8data) {
    TWDR0 = u8data;
    TWCR0 = (1<<TWINT)|(1<<TWEN);
    while ((TWCR0 & (1<<TWINT)) == 0);
}

uint8_t TWIMasterReadACK() {
    TWCR0 = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while ((TWCR0 & (1<<TWINT)) == 0);
    return TWDR0;
}

uint8_t TWIMasterReadNACK() {
    TWCR0 = (1<<TWINT)|(1<<TWEN);
    while ((TWCR0 & (1<<TWINT)) == 0);
    return TWDR0;
}

uint8_t TWIGetStatus() {
    return TWSR0 & 0xF8;
}

void TWISetBuffer(uint8_t *buffer_, uint8_t buffer_size_) {
    buffer_size = buffer_size_;
    buffer = buffer_;
}
