
#define F_CPU 8000000UL  // 8 MHz

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include "twi_interrupt.h"



uint8_t key_0[] = {
    0, 0,
    0, 3, 0, 42, 49, 4, 16, // Code
    0, 0,
    52,
    12, 26, 9, 3,       // Code
    0, 0, 0, 0, 0, 0, 0, 0,
    123, 124, 125, 126, 127, 128, 129, 130,
    0, 0, 0, 0, 0, 0, 0, 0,
    255, 255, 255, 255, 255, 255, 255, 255};



void blink() {
    PORTD ^= 0b00100000;
    _delay_ms(200);
    PORTD ^= 0b00100000;
    _delay_ms(200);
    PORTD ^= 0b00100000;
    _delay_ms(200);
    PORTD ^= 0b00100000;
}


void configure() {
    // Leds:
    //DDRD = 0b11100000;
    //PORTD = 0;
    //DDRB = 0b00000111;
    //PORTB = 0;
    //DDRC = 0;
    //DDRC = 0b00000010;
    DDRC = 0;
    PORTC = 255;
	
    // Interrupts
    sei();
    
    // Init TWI
    TWISetBuffer(key_0, 48);
    TWISlaveInit(87, 6); // Answer on 84 or 87
    
    
    // Ready to operate
    //blink();
}

ISR(TWI0_vect) {
    
    // own address has been acknowledged; prepare to receive
    if( TWIGetStatus() == TW_SR_SLA_ACK ) {
        my_twi_address = TWDR0;
    }
    
    // data receive successful
    if( TWIGetStatus() == TW_SR_DATA_ACK ){
        uint8_t mem_addr = TWDR0;
        next_byte_to_transmit = mem_addr;//(mem_addr < buffer_size) ? buffer[mem_addr] : 255;
    }
    
    // device has been addressed to be a transmitter
    if( TWIGetStatus() == TW_ST_DATA_ACK ){
        TWDR0 = next_byte_to_transmit;
    }

    // if none of the above apply prepare TWI to be addressed again
    TWCR0 = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);

    //TWCR0 = (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
}


int main(void) {
    configure();
}

