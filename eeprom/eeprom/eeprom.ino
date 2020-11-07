#define SCL 9
#define SDA 10

void _writeBit(bool bitv) {
  pinMode(SDA, OUTPUT);
  // signal set on clock-out
  digitalWrite(SDA, bitv ? HIGH : LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  Serial.print(bitv);
}

bool _readBit() {
  pinMode(SDA, INPUT_PULLUP);
  // Read on clock-out
  digitalWrite(SCL, HIGH);
  bool bitv = digitalRead(SDA);
  digitalWrite(SCL, LOW);
  Serial.print(bitv);
  return bitv;
}

void _start() {
  pinMode(SDA, OUTPUT);
  // Rising on clock-out
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
  
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW);
  Serial.println(F("start"));
}

void _stop() {
  pinMode(SDA, OUTPUT);
  // Falling on clock-out
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, LOW);
  Serial.println(F("stop"));
}

void _writeByte(byte bytev) {
  for (int x=7; x>=0; x--) {
    bool bitv = (bytev >> x) & 1;
    _writeBit(bitv);
  }
  Serial.print(F(" (0x"));
  Serial.print(bytev, HEX);
  Serial.println(F(")"));
}

void _readByte() {
  byte bytev = 0x00;
  for (int x=7; x>=0; x--) {
    bool bitv = _readBit();
    bytev += (bitv << x);
  }
  Serial.print(F(" (0x"));
  Serial.print(bytev, HEX);
  Serial.println(F(")"));
}

void _readAck() {
  Serial.print(F("Read ACK = "));
  _readBit();
  Serial.println();
}

void _writeAck(bool ack) {
  Serial.print(F("Write ACK = "));
  _writeBit(ack);
  Serial.println();
}

void _setDevAddr(bool isRead) {
  // Here we could take in additionally to operation also real device address 
  Serial.print(F("Device address = "));
  _writeByte(isRead ? 0xA1 : 0xA0);
  _readAck();
}

void _setAddr(byte addr) {  
  Serial.print(F("Address = "));
  _writeByte(addr);
  _readAck();
}

void _setValue(byte val) {
  Serial.print(F("Value = "));
  _writeByte(val);
  _readAck();
}

void _readValue() {
  Serial.print(F("Value = "));
  _readByte();
  _readAck();
}

void eepWriteByte(byte addr, byte val) {
  Serial.println(F("WRITE BYTE"));
  _start();
  _setDevAddr(false);
  _setAddr(addr);
  _setValue(val);
  _stop();
}

void eepReadRandom(byte addr) {
  Serial.println(F("READ RANDOM"));
  _start();
  _setDevAddr(false);
  _setAddr(addr);
  eepReadCurrent();
}

void eepReadCurrent() {
  Serial.println(F("READ CURRENT"));
  _start();
  _setDevAddr(true);
  _readValue();
  _stop();
}

void setup() {
  Serial.begin(115200);
  pinMode(SCL, OUTPUT);
  digitalWrite(SCL, LOW);

  Serial.println();
  Serial.println(F("== WRITE TO FIRST FIVE"));
  for (int addr=0; addr<5; addr++) {
    eepWriteByte(addr, 5 - addr);
  }

  Serial.println();
  Serial.println(F("== RANDOM READ ZERO"));
  eepReadRandom(0x00);
  
  Serial.println();
  Serial.println(F("== CURRENT READ NEXT FOUR"));
  eepReadCurrent();
  eepReadCurrent();
  eepReadCurrent();
  eepReadCurrent();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
