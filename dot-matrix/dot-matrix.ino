#define DATA_ROW 5 //serial input for register controlling rows WHITE
#define DATA_COL 4 //serial input for register controlling columns BLACK
#define CLK_ROW 3  //storage register clock BLUE
#define CLK_COL 6  //BLUE
#define LATCH 2    //shift register clock GREEN
#define BUTTON 7

int animationSpeed = 30;
int animation = 0;
int lastButtonState = LOW;
int previousChangeMillis = 0;

byte sClear[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

byte s0[8] = {
  0b11111111,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b11111111
};

byte s1[8] = {
  0b00000000,
  0b01111110,
  0b01111110,
  0b01111110,
  0b01111110,
  0b01111110,
  0b01111110,
  0b00000000,
};

byte s2[8] = {
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001,
};

byte s3[8] = {
  0b10101010,
  0b01010101,
  0b10101010,
  0b01010101,
  0b10101010,
  0b01010101,
  0b10101010,
  0b01010101
};

void matrixLED(byte s[8]) {
  //  Serial.println();

    for (int x = 0; x < 8; x++) {
      int col = 0xFF - (1 << (7 - x));
      shiftOut(DATA_ROW, CLK_ROW, LSBFIRST, s[x]);
      shiftOut(DATA_COL, CLK_COL, LSBFIRST, col);
      digitalWrite(LATCH, LOW);
      digitalWrite(LATCH, HIGH);
    }
}

void matrixDraw(byte s[8], int animationSpeed) {
  for (int x = 0; x < animationSpeed; x++) {
    matrixLED(s);  
  }
}

void matrixClear() {
    matrixLED(sClear);  
}

void setup() {
  Serial.begin(115200);
  pinMode(DATA_ROW, OUTPUT);
  pinMode(DATA_COL, OUTPUT);
  pinMode(CLK_ROW, OUTPUT);
  pinMode(CLK_COL, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(BUTTON, INPUT);

  digitalWrite(LATCH, LOW);
}

void playLineDownAnimation() {
  for (int frame = 0; frame < 8; frame++) {
    byte s[8];
    for (int row = 0; row < 8; row++) {
      byte rowValue = frame == row ? 0xFF : 0;
      s[row] = rowValue;
    }   
    matrixDraw(s, animationSpeed);
    matrixClear();
  }
}

void playLineVerticalBounceAnimation() {
  for (int frame = 0; frame < 16; frame++) {
    byte s[8];
    for (int row = 0; row < 8; row++) {
      byte rowValue = ((frame < 8) ? (frame == row) : ((15 - frame) == row)) ? 0xFF : 0;
      s[row] = rowValue;
    }
    matrixDraw(s, animationSpeed);
    matrixClear();
  }
}

void playLineRightAnimation() {
  for (int frame = 0; frame < 8; frame++) {
    byte s[8];
    for (int row = 0; row < 8; row++) {
      byte rowValue = 1 << (frame);
      s[row] = rowValue;
    }   
    
    matrixDraw(s, animationSpeed);
    matrixClear();
  }
}

void playLineHorizontalBounceAnimation() {
  for (int frame = 0; frame < 16; frame++) {
    byte s[8];
    for (int row = 0; row < 8; row++) {
      byte rowValue = (frame < 8) ? 1 << (frame) : 1 << (15 - frame);
      s[row] = rowValue;
    }   
    
    matrixDraw(s, animationSpeed);
    matrixClear();
  }
}

void playLineBounceAnimation() {
  for (int frame = 0; frame < 16; frame++) {
    byte s[8];
    for (int row = 0; row < 8; row++) {
      byte rowValue = (frame < 8) ? 1 << (frame) : 1 << (15 - frame);
      rowValue = ((frame < 8) ? (frame == row) : ((15 - frame) == row)) ? 0xFF : rowValue;
      s[row] = rowValue;
    }
   
    matrixDraw(s, animationSpeed);
    matrixClear();
  }
}

void playSquareShrinkAnimation() {
  for (int frame = 0; frame < 4; frame++) {
    byte s[8];
    //    Serial.println();
    for (int row = 0; row < 8; row++) {
      //      byte rowValue = (row >= frame && row <= (7-frame)) ? (1 << frame) + (1 << 7 - frame) : 0;
      byte rowValue = (row >= frame && row <= (7 - frame)) ? ((0xFF >> frame * 2) << frame) : 0;
      //      Serial.println(rowValue, BIN);
      //      byte rowValue = (frame < 8) ? 1 << (frame) : 1 << (15-frame);
      //      rowValue = ((frame < 8) ? (frame == row) : ((15-frame) == row)) ? 0xFF : rowValue;
      s[row] = rowValue;
    }
    
    matrixDraw(s, animationSpeed);
    matrixClear();
  }
}

void loop() {
  int now = millis();

  if (now - previousChangeMillis > 5000) {
    previousChangeMillis = now;
    animation++;
    if (animation > 5) {
      animation = 0;
    }
  }
  
//  int buttonReading = digitalRead(BUTTON);
//  if (buttonReading == HIGH && lastButtonState == LOW) {
//    animation++;
//    if (animation > 5) {
//      animation = 0;
//    }
//  }
//  lastButtonState = buttonReading;

  if (animation == 0) {
    playLineDownAnimation();
  } else if (animation == 1) {
    playLineVerticalBounceAnimation();
  } else if (animation == 2) {
    playLineRightAnimation();
  } else if (animation == 3) {
    playLineHorizontalBounceAnimation();
  } else if (animation == 4) {
    playLineBounceAnimation();
  } else if (animation == 5) {
    playSquareShrinkAnimation();
  }
  
  //  for (int x=0 ; x<20 ; x++) {
  //    matrixLED(s0);
  //  }
  //  delay(5);
  //
  //  for (int x=0 ; x<20 ; x++) {
  //    matrixLED(s3);
  //  }
  //  delay(5);
}
