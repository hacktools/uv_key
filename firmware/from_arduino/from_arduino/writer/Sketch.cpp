/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <Wire.h>
//Beginning of Auto generated function prototypes by Atmel Studio
void requestEvent_emulator();
void receiveEvent_emulator(int howMany);
//End of Auto generated function prototypes by Atmel Studio



int next;

int key_0[] = {
    0, 0, 
    0, 3, 0, 42, 49, 4, 16, // Code
    0, 0, 
    52, 
    12, 26, 9, 3,       // Code 
    0, 0, 0, 0, 0, 0, 0, 0, 
    123, 124, 125, 126, 127, 128, 129, 130,
    0, 0, 0, 0, 0, 0, 0, 0, 
    255, 255, 255, 255, 255, 255, 255, 255};

int key_backup[] = {
    0, 0, 
    0, 3, 0, 42, 49, 4, 16, // Code
    0, 0, 
    42, 
    10, 26, 9, 3,       // Code 
    0, 0, 0, 0, 0, 0, 0, 0, 
    123, 124, 125, 126, 127, 128, 129, 130,
    0, 0, 0, 0, 0, 0, 0, 0, 
    255, 255, 255, 255, 255, 255, 255, 255};


int tx;
int rx;
int x;


void emulator_setup() {
  // PC0 = KEY_PWR
  // PC1 = KEY_CS
  // PC4 = KEY_DATA
  // PC5 = KEY_CLK
  // PC6 = RES
  DDRC  = 0b01000000;
  PORTC = 0b00000000;
  DDRB  = 0b11111111;
  PORTB = 0b00000000;

  // 87 = 0b01010111
  // 84 = 0b01010100
  // 83 = 0b01010011
  // 6  = 0b00000110
  TWAMR = 6;  // 84 or 87
  //TWAMR = 255;  // ALL
  //Serial.begin(9600);
  Wire.begin(87);
  Wire.onRequest(requestEvent_emulator);
  Wire.onReceive(receiveEvent_emulator);
  tx = 0;
  rx = 0;

  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  sleep_mode();
}

void dumper_setup() {

}

void sniffer_setup() {

}


void setup() {
  emulator_setup();
}

void loop() {
	delay(600);
}

void requestEvent_emulator() {
  //sleep_disable();
  PORTB = 0b00100000;

  if(rx == 1)
    Wire.write(next);

  if (rx > 1) {
    if (x == 6)
      Wire.write(6);
    else
      Wire.write("hello ");
  }

  tx += 1;
  rx = 0;

  PORTB = 0b00000000;
}


void receiveEvent_emulator(int howMany) {
  //sleep_disable();
  PORTB = 0b00100000;

  while (1 < Wire.available()) {
    char c = Wire.read();
    rx += 1;
  }
  x = Wire.read();

  
  rx += 1;
  tx = 0;
  
  if (x > 48) {
    next = 255;
    return;
  }
  
  next = key_0[x];

  PORTB = 0b00000000;
}





