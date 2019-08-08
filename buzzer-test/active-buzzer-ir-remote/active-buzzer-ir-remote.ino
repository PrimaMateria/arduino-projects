#include <IRremote.h>

int RECV_PIN = 11;
int PIN_SPEAKER = 4;
int length = 25;
char notes[] = "ggagCbggagDCggGECbafffECDC";
int beats[] = {1,1,2,2,2,4,1,1,2,2,2,4,1,1,2,2,2,2,2,2,1,1,2,2,2,4,1};
int tempo = 300;
int noteIndex = 0;
unsigned long time;

IRrecv irrecv(RECV_PIN);

decode_results results;

void playTone(int tone, int duration) {
  for (long i=0; i < duration * 1000L; i += tone *2) {
    digitalWrite(PIN_SPEAKER, HIGH);
    delayMicroseconds(tone);
    digitalWrite(PIN_SPEAKER, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G'};
  int tones[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956, 853, 759, 716, 637, 568};

  for (int i = 0; i < 12; i++) {
    if (names[i] == note) {
      playTone(tones[i]*2, duration);
    }
  }
}

void lowerTempo() {
  tempo += 50;
  if (tempo > 1000) {
    tempo = 1000;
  }
}

void increaseTempo() {
  tempo -= 50;
  if (tempo < 0) {
    tempo = 0;
  }
}

void setup() {
  pinMode(PIN_SPEAKER, OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    switch(results.value) {
      case 0xFFE01F: //-
        lowerTempo();
        Serial.println(tempo);
        break;
      case 0xFFA857: //+
        increaseTempo();
        Serial.println(tempo);
        break;
    }

    irrecv.resume();
  }

//  Serial.println("Start");
  time = millis();
  if (time / (tempo * (noteIndex + 1)) == 1) {
    Serial.print(noteIndex);
    Serial.println(" play");
    noteIndex += 1;
  }

  delay(10);
  
  /*for (int i = 0; i < length; i++) {*/
    /*if (notes[i] == ' ') {*/
      /*delay(beats[i] * tempo);*/
    /*} else {*/
      /*playNote(notes[i], beats[i] * tempo);*/
    /*}*/

    /*delay (tempo / 2);*/
  /*}*/
}
