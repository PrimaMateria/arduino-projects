#include <WiFiManager.h>
#include <WiFi.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>

const uint32_t kBaudRate = 115200;
const uint16_t kReceiverPin = 14;
const uint16_t kTransmitterPin = 4;

IRrecv irrecv(kReceiverPin);
decode_results results;

int port = 3000;
IPAddress server(192,168,178,25);
WiFiClient client;

unsigned long currentMillis;
unsigned long timer;
const unsigned long beamPeriod = 2000;
const unsigned long receiverPeriod = 700;
const unsigned long pausePeriod = 10000;
bool beamDetected;
bool isBeamUp;

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
  wm.autoConnect("NodeMCU");
  Serial.println("Connected");
}

void pingServer() {
  if (client.connect(server, port)) {
    Serial.println("Client connected to server");
    client.println("POST /post HTTP/1.0");
    client.println();
  }  
}

void setup() {
  Serial.begin(kBaudRate);
  while(!Serial) {
    delay(50);
  }


  pinMode(kTransmitterPin, OUTPUT);
  irrecv.enableIRIn();
  
  //startWifiManager();
  //pingServer();
  
  Serial.println("Setup finished");
}

void startBeam() {
  Serial.println("Start beam");
  digitalWrite(kTransmitterPin, HIGH);
  isBeamUp = true;
}

void stopBeam() {
  Serial.println("Stop beam");
  digitalWrite(kTransmitterPin, LOW);
  isBeamUp = false;
}

bool detectBeam() {
  bool isBeamDetected = false;  
  if (irrecv.decode(&results)) {
    isBeamDetected = true;
    irrecv.resume();
  }
  return isBeamDetected;
}

void loop() {
  currentMillis = millis();

  bool isFirstRun = timer == 0;
  bool isPauseOver = (currentMillis - timer) >= (beamPeriod + pausePeriod);
  
  if ((isFirstRun || isPauseOver) && !isBeamUp) {
    startBeam();
    beamDetected = false;
    timer = currentMillis;
  }

  bool receiverStart = (currentMillis - timer) >= (beamPeriod - receiverPeriod) / 2;
  bool receiverStop = (currentMillis - timer) >= ((beamPeriod - receiverPeriod) / 2) + receiverPeriod;
  if (receiverStart && !receiverStop) {
    beamDetected = beamDetected || detectBeam();    
  }

  bool beamStop = ((currentMillis - timer) >= beamPeriod);
  if (beamStop && isBeamUp) {
    stopBeam();
  
    Serial.print("Beam detected = ");
    Serial.println(beamDetected);
  }
  
  delay(20);
}
