#include <Wire.h>
#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Key power
  pinMode(17, OUTPUT); // A3
  digitalWrite(17, HIGH);

  // LED
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop () {
  byte addr[2];
  int n = 0;
  Serial.println("\n-------------------\n");
  for (byte i = 0; i < 255; ++i) {
    Serial.print(i);
    Serial.print(": ");

    for (byte j = 0; j < 255; ++j) {
      Wire.beginTransmission(82);
      addr[0] = i;
      addr[1] = j;
      Wire.write(addr, 2);  
      Wire.endTransmission();
      Wire.requestFrom(82, 1);
      byte c = Wire.read();
      Serial.print(c, DEC);
      Serial.print("; ");
      n++;
      //if (n % 48 == 0)
      //  Serial.println("");
    }
    n = 0;
    Serial.println("");
  }  
}

