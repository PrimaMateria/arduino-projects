//#include <WiFiManager.h>
//#include <WiFi.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#define IRR 2 //G2
#define IRS 4 //G4

const uint32_t kBaudRate = 115200;
const uint16_t kRecvPin = 14;
const uint16_t kIrLedPin = 2;
const uint16_t kCaptureBufferSize = 1024;
const uint16_t kTimeout = 50;
const uint16_t kFrequency = 38000;

IRsend irsend(kIrLedPin);
//IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, false);
decode_results results;

//int port = 3000;
//IPAddress server(192,168,178,25);
//WiFiClient client;

void setup() {

//  pinMode(kIrLedPin, OUTPUT);
  pinMode(kRecvPin, INPUT);
  irsend.enableIROut(38);
  
  Serial.begin(kBaudRate);
  while(!Serial) {
    delay(50);
  }
  Serial.println();
  Serial.print("DumbIRRepeater is now running and waiting for IR input on Pin ");
  Serial.println(kRecvPin);
  Serial.print("and will retransmit it on Pin ");
  Serial.println(kIrLedPin);
  
//  WiFiManager wm;
//  wm.autoConnect("NodeMCU");
//  Serial.println("Connected");
//
//  if (client.connect(server, port)) {
//    Serial.println("Client connected to server");
//    client.println("POST /post HTTP/1.0");
//    client.println();
//  }
////
//  pinMode(IRS, OUTPUT);
//  pinMode(IRR, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.println("Idle...");

//  int testPin = kIrLedPin;
//  pinMode(testPin,OUTPUT);
//  digitalWrite(testPin, HIGH);
//  delay(500);  
//  digitalWrite(testPin, LOW);
//  delay(500);  


//  irsend.space(0);
//  delay(1);
  irsend.mark(1000/(38*2));
//
//  
//  digitalWrite(kIrLedPin, LOW);
//  delay(1);
//  digitalWrite(kIrLedPin, HIGH);
//  

   int recv = digitalRead(kRecvPin);
   Serial.println(recv);
   delay(50);

  
}
