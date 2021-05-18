void debugPin(int testPin) {
  pinMode(testPin,OUTPUT);
  digitalWrite(testPin, HIGH);
  delay(500);  
  digitalWrite(testPin, LOW);
  delay(500);    
}

void analogDebugPin(int testPin) {
  pinMode(testPin,OUTPUT);
  for (int i=0; i<256; i++) {
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void startWifiManager() {
  WiFiManager wm;
  wm.setConfigPortalTimeout(kAutoConfigTimeout);
  wm.autoConnect("NodeMCU");
  Serial.println("Connected");
}
