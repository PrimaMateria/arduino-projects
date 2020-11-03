#include <extEEPROM.h>

const uint32_t totalKBytes = 2;
const uint8_t chunkSize = 4;

extEEPROM eep(kbits_16, 1, 16);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println(F("---"));
  uint8_t eepStatus = eep.begin(extEEPROM::twiClock400kHz);
  if (eepStatus) {
    Serial.print(F("extEEPROM.begin() failed, status = "));
    Serial.println(eepStatus);
    while (1);
  }

  //totalKBytes*1024
  dump(0, 32);

  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void dump(uint32_t startAddr, uint32_t nBytes) {
  Serial.print(F("EEPROM DUMP 0x"));
  Serial.print(startAddr, HEX);
  Serial.print(F(" 0x"));
  Serial.print(nBytes, HEX);
  Serial.print(F(" "));
  Serial.print(startAddr);
  Serial.print(F(" "));
  Serial.println(nBytes);

  uint32_t nRows = (nBytes + 15) >> 4;

  uint8_t d[16];
  for(uint32_t r = 0; r < nRows; r++) {
    uint32_t a = startAddr + 16 * r;
    eep.read(a, d, 16);

    Serial.print(F("0x"));
    if (a < 16 * 16 * 16) Serial.print(F("0"));
    if (a < 16 * 16) Serial.print(F("0"));
    if (a < 16) Serial.print(F("0"));
    Serial.print(a, HEX);
    Serial.print(F(" "));

    for(int c = 0; c < 16; c++) {
      if (d[c] < 16) Serial.print(F("0"));
      Serial.print(d[c], HEX);
      Serial.print(c == 7 ? "  ": " ");
    }
    Serial.println(F(""));
  }
}
