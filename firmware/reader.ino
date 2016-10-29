#include <Wire.h>
#include <EEPROM.h>


byte keyDB[10][57];
byte keys;

int ce_pin = 17;

void setup() {
  //CE
  pinMode(ce_pin, INPUT);
  digitalWrite(ce_pin, LOW);
  // LED
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  Serial.begin(9600);
  Wire.begin();

  // Key power
  pinMode(17, OUTPUT); // A3
  digitalWrite(17, HIGH);




  //EEPROM.write(0, 0);

  restoreFromEEPROM();

  digitalWrite(13, HIGH);
  delay (500);
  digitalWrite(13, LOW);
}


void blink() {
  digitalWrite(13, HIGH);
  delay (300);
  digitalWrite(13, LOW);
}


void restoreFromEEPROM() {
  keys = EEPROM.read(0);
  Serial.print("Keys stored: ");
  Serial.println(keys, DEC);

  if (keys == 0) return;

  for (int i = 0; i < keys; ++i) {
    Serial.print("Reading key ");
    Serial.print(i + 1, DEC);
    Serial.print("/");
    Serial.println(keys, DEC);
    
    for (int j = 0; j < 57; ++j) {
      keyDB[i][j] = EEPROM.read(i*57 + j + 1);
    }
  }
}



// ---------------------------------------------

void loop() {
  byte key[57];
  readKey(key);

  if (!keyInDB(key))
    addKey(key);

  printDB();
}


bool keyInDB(byte key[57]) {
  for (int i = 0; i < keys; ++i) {
    bool match = true;
    for (int j = 0; j < 57; ++j) {
      if (key[j] != keyDB[i][j]) {
        match = false;
        break;
      }
    }
    if (match) return true;
  }
  return false;
}


void addKey(byte key[57]) {
  Serial.println("Adding key:");
  printKey(key);

  keys++;
  EEPROM.write(0, keys);
   
  for (int j = 0; j < 57; ++j) {
    keyDB[keys - 1][j] = key[j];
    EEPROM.write((keys-1)*57 + j + 1, key[j]);
  }
  blink();
  Serial.println("Key added");
}


void printKey(byte key[57]) {
  for (int i = 0; i < 57; ++i) {
    Serial.println(key[i], DEC);
  }
}

void printEEPROM() {
  int n = EEPROM.read(0);
  Serial.print("Keys stored: ");
  Serial.println(n, DEC);

  if (n == 0) return;

  for (int j = 0; j < 57; ++j) {
    for (int i = 0; i < n; ++i) {
      Serial.print(EEPROM.read(i*57 + j + 1));
      Serial.print("\t");
    }
    Serial.println("");
  }
}

void printDB() {
  Serial.print("Keys stored: ");
  Serial.println(keys, DEC);

  if (keys == 0) return;

  for (int j = 0; j < 57; ++j) {
    for (int i = 0; i < keys; ++i) {
      Serial.print(keyDB[i][j], DEC);
      Serial.print("\t");
    }
    Serial.println("");
  }
}

bool readKey(byte key[57]) {
  Serial.write("\n\nStarting new log:\n");

  // Section I
  key[0] = OneByte(87, 232); // 255
  key[1] = OneByte(87, 233); // 255

  // Lock reprogramming request?
  Wire.beginTransmission(83);
  Wire.endTransmission();
  delayMicroseconds(400);
  // ---------------------------

  key[2] = OneByte(84, 6);   // 48
  delay (19);
  // End of section I
  //

  // Section II
  key[3] = OneByte(87, 232); // 255
  key[4] = OneByte(87, 233); // 255
  key[5] = OneByte(87, 233); // 255
  key[6] = OneByte(87, 234); // 255

  for (int i=0; i < 24; i++)
    key[i + 7] = OneByte(84, i);
  delay (300);
  // End of section II
  //

  // Section III
  key[31] = OneByte(87, 232); // 255
  key[32] = OneByte(87, 233); // 255

  for (int i=24; i < 48; i++)
    key[i + 9] = OneByte(84, i);
  // End of section III
  //

  delay (800);
}

char OneByte(int addr, int val) {
  Wire.beginTransmission(addr);
  Wire.write(val); 
  Wire.endTransmission();
  
  delayMicroseconds(40);
  
  Wire.requestFrom(addr, 1);
  byte c = Wire.read();
  //Serial.println(c, DEC);
  return c;
}

