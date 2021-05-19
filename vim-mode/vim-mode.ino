#include <WiFiManager.h>
#include <WiFi.h>
#include <analogWrite.h>

const uint16_t kAutoConfigTimeout = 10;
const uint32_t kBaudRate = 115200;

/**
 * 0 - none          - black
 * 1 - Normal mode   - blue
 * 2 - Visual mode   - red
 * 3 - Insert mode   - green
 * 4 - Cmdline mode  - violet
 * 5 - Terminal mode - yellow
 */
const uint16_t colors[6][3] = {
  {   0,   0,   0 },
  {   0,   0, 255 },
  { 255,   0,   0 },
  {   0, 255,   0 },
  { 255,   0, 255 },
  { 255, 255,   0 }
};

const uint16_t ledR = 14;
const uint16_t ledG = 13;
const uint16_t ledB = 12;

WiFiServer server(80);

const unsigned long pollPeriod = 2000;

void setup() {
  Serial.begin(kBaudRate);
  while(!Serial) {
    delay(50);
  }
 
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

  analogWrite(ledR,255);
  analogWrite(ledG,255);
  analogWrite(ledB,255);
  
  startWifiManager();
  server.begin();
  Serial.println("Setup finished");
}

void startWifiManager() {
  WiFiManager wm;
  wm.setConfigPortalTimeout(kAutoConfigTimeout);
  wm.autoConnect("NodeMCU");
  Serial.println("Connected");
}

void indicateMode(int mode) {
  analogWrite(ledR,colors[mode][0]);
  analogWrite(ledG,colors[mode][1]);
  analogWrite(ledB,colors[mode][2]);
}

void listen() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("client connected");
    bool currentLineIsBlank = true;
    String header = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          int modeIndex = header.indexOf("mode:");
          if (modeIndex != -1) {
            int mode = header.charAt(modeIndex + 6) - '0';
            Serial.println(mode);
            indicateMode(mode);
          }
          client.println("HTTP/1.1 200 OK");
          client.println("Connection: close");
          break;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("client disconnected");
  } else {
    /*Serial.println("client not avaialble");*/
  }
}

void loop() {
  listen();
}
