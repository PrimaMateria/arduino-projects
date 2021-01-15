const uint8_t irReceiverPin=2;
const uint8_t irEmitterPin=0;
const uint8_t ledPin=1;

unsigned long currentMillis;
unsigned long timer;
const unsigned long beamPeriod = 2000;
const unsigned long receiverPeriod = 700;
const unsigned long pausePeriod = 3000;
const unsigned long pulsePeriod = 200;

bool isBeamDetected;
bool isBeamUp;
bool isIrEmitterOn;
uint16_t checksCount = 0;
const uint8_t kMaxChecks = 3;
 
void setup() {
 pinMode(irReceiverPin,INPUT);
 pinMode(ledPin, OUTPUT);
 pinMode(irEmitterPin, OUTPUT);

 setIrEmitter(true);
}

void debug(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);   
  delay(1000);               
  digitalWrite(pin, LOW);    
  delay(1000);
}

void setSignal(bool isOn) {
  digitalWrite(ledPin, isOn ? HIGH : LOW);
}

void setIrEmitter(bool isOn) {
  digitalWrite(irEmitterPin, isOn ? HIGH: LOW);
  isIrEmitterOn = isOn;
}

bool detectIrBeam() {
  bool isBeamDetected = false;  
  if (digitalRead(irReceiverPin) == LOW) {
    isBeamDetected = true;
  }
  return isBeamDetected;
}
 
void check() {  
  currentMillis = millis();

  bool isFirstRun = timer == 0;
  bool isPauseOver = (currentMillis - timer) >= (beamPeriod + pausePeriod);
  
  if ((isFirstRun || isPauseOver) && !isBeamUp) {    
    isBeamDetected = false;
    isBeamUp = true;
    timer = currentMillis;
  }

  if (isBeamUp) {
    bool isPulseUp = (((currentMillis - timer) / pulsePeriod) % 2) == 0;
    if (isPulseUp && !isIrEmitterOn) {
      setIrEmitter(true);
    } else if (!isPulseUp && isIrEmitterOn) {
      setIrEmitter(false);
    }
  }

  bool receiverStart = (currentMillis - timer) >= (beamPeriod - receiverPeriod) / 2;
  bool receiverStop = (currentMillis - timer) >= ((beamPeriod - receiverPeriod) / 2) + receiverPeriod;
  if (receiverStart && !receiverStop) {
    isBeamDetected = isBeamDetected || detectIrBeam();    
  }

  bool beamStop = ((currentMillis - timer) >= beamPeriod);
  if (beamStop && isBeamUp) { 
    setIrEmitter(false);
    isBeamUp = false;
    checksCount++;
  }
}

void wakeUpBattery() {
  for (int i=0; i<3; i++) {
    setSignal(true);
    delay(1000);
    setSignal(false);
    delay(100);
  }
  setSignal(!isBeamDetected);
}
 
void loop() {
  if (checksCount < kMaxChecks) {
    check();
  } else {
    setSignal(!isBeamDetected);
    //TODO: deep sleep
    delay(20000);
    wakeUpBattery();
    checksCount = 0;
    isBeamDetected = false;
  }
    
  delay(20);
}
