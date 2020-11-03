#define SCL 9
#define SDA 10

void sendStart() {
  pinMode(SDA, OUTPUT);
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
  delay(20);
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW);
  Serial.println("START sent");
}

void sendStop() {
  pinMode(SDA, OUTPUT);
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  delay(20);
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, LOW);
  Serial.println("STOP sent");
}

void send8Bits(int value) {
  pinMode(SDA, OUTPUT);
  Serial.print("Writing: ");
  for (int x=7; x>=0; x--) {
    boolean xthBit = (value >> x) & 1;
    if (xthBit != 0) {
      Serial.print("1");
      digitalWrite(SDA, HIGH);
    } else {
      Serial.print("0");
      digitalWrite(SDA, LOW);
    }
    delay(20);
    digitalWrite(SCL, HIGH);
    delay(20);
    digitalWrite(SCL, LOW);
    delay(20);
  }
  Serial.print(F(" (0x"));
  Serial.print(value, HEX);
  Serial.print(F(")"));
  Serial.println();
}

void readAcknowledge() {
  pinMode(SDA, INPUT);
  digitalWrite(SCL, HIGH);
  int acknowledge = digitalRead(SDA);
  digitalWrite(SCL, LOW);
  Serial.print("Read acknowledge=");
  Serial.println(acknowledge);
}

void sendAcknowledge(boolean acknowledge) {
  pinMode(SDA, OUTPUT);
  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, acknowledge ? HIGH : LOW);
  digitalWrite(SCL, LOW);
  Serial.print("Send acknowledge=");
  Serial.println(acknowledge);
}

void read8Bits() {
  pinMode(SDA, INPUT);
  Serial.print("Reading: ");
  for (int x=7; x>=0; x--) {
    digitalWrite(SCL, HIGH);
    digitalWrite(SCL, LOW);
    delay(20);
    int xthBit = digitalRead(SDA);
    Serial.print(xthBit);
  }
  Serial.println();
}

void eepromWrite(int address, int value) {
  sendStart();
  send8Bits(B10100000);
  readAcknowledge();
  send8Bits(address);
  readAcknowledge();
  send8Bits(value);
  readAcknowledge();
  sendStop();
}

void eepromRandomRead(int address) {
  sendStart();
  send8Bits(B10100000);
  readAcknowledge();
  send8Bits(address);
  readAcknowledge();
  sendStart();
  send8Bits(B10100001);
  readAcknowledge();
  read8Bits();
  sendAcknowledge(false);
  sendStop();
}

void setup() {
  Serial.begin(115200);
  pinMode(SCL, OUTPUT);
  pinMode(SDA, OUTPUT);
  digitalWrite(SCL, LOW);
  digitalWrite(SDA, LOW);

  Serial.println("---");
  // 1010 3<DEVICE_ADR> 1<W0/R1>

  // Write 10101010 to 0
  eepromWrite(0x01, 0xFF);

  // Random Read 0
//   eepromRandomRead(0x01);

//   sendStart();
//   send8Bits(B10100000);
//   readAcknowledge();
//   read8Bits();
//   sendAcknowledge(true);
//   read8Bits();
//   sendAcknowledge(true);
//   read8Bits();
//   sendAcknowledge(true);
//   read8Bits();
//   sendAcknowledge(true);
//   read8Bits();
//   sendAcknowledge(true);
//   sendStop();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
