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


void setup() {
  Serial.begin(kBaudRate);
  while(!Serial) {
    delay(50);
  }
 
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  setColor(0, 0, 0);

  startWifiManager();
  server.begin();
  Serial.println("Setup finished");
}

void startWifiManager() {
  WiFiManager wm;
  wm.setConfigPortalTimeout(kAutoConfigTimeout);
  wm.setAPCallback(configModeCallback);
  wm.autoConnect("NodeMCU");
  Serial.println("Connected");
}

void configModeCallback(WiFiManager *wm) {
  // blink red 3 times
  for(int i=0; i<3; i++) {
    indicateMode(2);
    delay(1000);
    indicateMode(0);
    delay(500);
  }
  // stay red
  indicateMode(2);
}

void setColor(uint16_t red, uint16_t green, uint16_t blue) {
  analogWrite(ledR, red);
  analogWrite(ledG, green);
  analogWrite(ledB, blue);
}

void indicateMode(int mode) {
  setColor(colors[mode][0], colors[mode][1], colors[mode][2]);
}

unsigned long lightShowCycleStart;
uint16_t lightShowColors[3] = { 255, 0, 0 };
uint16_t decColor = 0;
uint16_t incColor;
uint16_t colorSwapCycleCounter = 0;
uint16_t crossfadeCycleCounter = 0;

void lightshow() {
  if (lightShowCycleStart == 0) {
    lightShowCycleStart = millis();
  }

  long passedMillis = millis() - lightShowCycleStart;

  if (passedMillis > 50) {
    lightShowCycleStart = millis();

    if (crossfadeCycleCounter == 0) {
      decColor = colorSwapCycleCounter % 3;
      incColor = decColor == 2 ? 0 : decColor + 1;
      colorSwapCycleCounter++;
    }
    
    if (crossfadeCycleCounter < 255) {
      lightShowColors[decColor] -= 1;
      lightShowColors[incColor] += 1;
      setColor(lightShowColors[0], lightShowColors[1], lightShowColors[2]);
      crossfadeCycleCounter++;
    } else {
      crossfadeCycleCounter = 0;
    }
  }
}

unsigned long modeCycleStart = 0;

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
            modeCycleStart = millis();
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

  if (modeCycleStart == 0 || millis() - modeCycleStart > 5000) {
    lightshow();
  }
}
