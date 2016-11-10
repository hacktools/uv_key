#include <Wire.h>

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

int ce_pin = 17;

void setup() {
  pinMode(ce_pin, OUTPUT);
  digitalWrite(ce_pin, HIGH);
  
  TWAMR = 6;  // 84 or 87
  //TWAMR = 255;  // ALL
  Serial.begin(9600);
  Wire.begin(87);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  tx = 0;
  rx = 0;
}

void loop() {
  digitalWrite(ce_pin, LOW);
  delay(800);
}

void requestEvent() {
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
}


void receiveEvent(int howMany) {
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
}





