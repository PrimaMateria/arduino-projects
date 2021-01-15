#include <WiFiManager.h>
#include <WiFi.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>

const uint16_t kAutoConfigTimeout = 10;
const uint32_t kBaudRate = 115200;
const uint16_t kReceiverPin = 14;
const uint16_t kTransmitterPin = 4;
const uint16_t kMaxChecks = 3;

IRrecv irrecv(kReceiverPin);
decode_results results;

int port = 3000;
IPAddress server(192,168,178,25);
WiFiClient client;

unsigned long currentMillis;
unsigned long timer;
const unsigned long beamPeriod = 2000;
const unsigned long receiverPeriod = 700;
const unsigned long pausePeriod = 3000;
const unsigned long pulsePeriod = 200;
bool beamDetected;
bool isBeamUp;
bool isLedOn;
uint16_t checksCount = 0;

// this is temporary helper to detect the PIN on the board by sending repeating signal to it
void debugPin(int testPin) {
  pinMode(testPin,OUTPUT);
  digitalWrite(testPin, HIGH);
  delay(500);  
  digitalWrite(testPin, LOW);
  delay(500);    
}

void startWifiManager() {
  WiFiManager wm;
  wm.setConfigPortalTimeout(kAutoConfigTimeout);
  wm.autoConnect("NodeMCU");
  Serial.println("Connected");
}

void pingServer() {
  if (client.connect(server, port)) {
    Serial.println("Sending POST /post/ping");
    client.println("POST /post/ping HTTP/1.0");
    client.println();
  } else {
    Serial.print("Failed to connect client");
  }
}

void putPostStatus() {
  if (client.connect(server, port)) {
    Serial.println("Sending PUT /post/status/");
    client.print("PUT /post/status/");
    if (beamDetected == 1) {
      client.print("false");
    } else {
      client.print("true");
    }
    client.println(" HTTP/1.0");
    client.println();
  } else {
    Serial.print("Failed to connect client");
  }
}

void setup() {
  Serial.begin(kBaudRate);
  while(!Serial) {
    delay(50);
  }
 

  pinMode(kTransmitterPin, OUTPUT);
  irrecv.enableIRIn();
  checksCount = 0;
  
  startWifiManager();
  pingServer();
  
  Serial.println("Setup finished");
}

void turnOnLed() {
  Serial.println("LED ON");
  digitalWrite(kTransmitterPin, HIGH);
  isLedOn = true;
}

void turnOffLed() {
  Serial.println("LED OFF");
  digitalWrite(kTransmitterPin, LOW);
  isLedOn = false;
}

bool detectBeam() {
  bool isBeamDetected = false;  
  if (irrecv.decode(&results)) {
    isBeamDetected = true;
    irrecv.resume();
  }
  return isBeamDetected;
}

void check() {  
  currentMillis = millis();

  bool isFirstRun = timer == 0;
  bool isPauseOver = (currentMillis - timer) >= (beamPeriod + pausePeriod);
  
  if ((isFirstRun || isPauseOver) && !isBeamUp) {    
    Serial.println("Start beam");
    beamDetected = false;
    isBeamUp = true;
    timer = currentMillis;
  }

  if (isBeamUp) {
    bool isPulseUp = (((currentMillis - timer) / pulsePeriod) % 2) == 0;
    if (isPulseUp && !isLedOn) {
      turnOnLed();
    } else if (!isPulseUp && isLedOn) {
      turnOffLed();
    }
  }

  bool receiverStart = (currentMillis - timer) >= (beamPeriod - receiverPeriod) / 2;
  bool receiverStop = (currentMillis - timer) >= ((beamPeriod - receiverPeriod) / 2) + receiverPeriod;
  if (receiverStart && !receiverStop) {
    beamDetected = beamDetected || detectBeam();    
  }

  bool beamStop = ((currentMillis - timer) >= beamPeriod);
  if (beamStop && isBeamUp) { 
    Serial.println("Stop beam");
    turnOffLed();
    isBeamUp = false;
    checksCount++;
  
    Serial.print("Beam detected = ");
    Serial.println(beamDetected);
  }
}

void loop() {

  if (checksCount < kMaxChecks) {
    check();
  } else {
    Serial.println("Checks done, sending status to server and enabling deep sleep");
    putPostStatus();
    //TODO: deep sleep
    delay(20000);
    checksCount = 0;
  }
    
  delay(20);
}
