#include <IRremote.h>

int RECV_PIN = 11;
int PIN_SPEAKER = 4;
int length = 25;
char notes[] = "ggagCbggagDCggGECbafffECDC";
int beats[] = {1,1,2,2,2,4,1,1,2,2,2,4,1,1,2,2,2,2,2,2,1,1,2,2,2,4,1};
int tempo = 300;

int noteIndex = 0;
boolean next;
long unsigned int noteDuration;
long unsigned int noteStartTime;
char note;

IRrecv irrecv(RECV_PIN);

decode_results results;

void playTone(int tone) {
  digitalWrite(PIN_SPEAKER, HIGH);
  delayMicroseconds(tone);
  digitalWrite(PIN_SPEAKER, LOW);
  delayMicroseconds(tone);
}

void playNote(char note) {
  char names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G'};
  int tones[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956, 853, 759, 716, 637, 568};

  for (int i = 0; i < 12; i++) {
    if (names[i] == note) {
      playTone(tones[i]*2);
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
  next = true;
}


void loop() {
  
  // ----------------------------------------
  // IR Remote
  // ----------------------------------------

  if (irrecv.decode(&results)) {
    /*Serial.println(results.value, HEX);*/
    switch(results.value) {
      case 0xFFE01F: //-
        lowerTempo();
        /*Serial.println(tempo);*/
        break;
      case 0xFFA857: //+
        increaseTempo();
        /*Serial.println(tempo);*/
        break;
    }

    irrecv.resume();
  }

  // ----------------------------------------
  // Buzzer
  // ----------------------------------------

  // Set next note
  if (next == true) {
    noteStartTime = millis();
    noteDuration = tempo * beats[noteIndex];
    note = notes[noteIndex];

    /*Serial.print("Playing ");*/
    /*Serial.print(note);*/
    /*Serial.println(noteDuration);*/

    noteIndex++;
    if (noteIndex >= length) {
      noteIndex = 0;
    }
    next = false;
  }
  
  // Keep playing note, when finished make a break and request next note
  if (millis() < noteStartTime + noteDuration) {
    playNote(note);
  } else {
    delay(tempo);
    next = true;
  }

}
