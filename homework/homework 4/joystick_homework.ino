// Define Connections to 74HC595
#include <EEPROM.h>

// system states, it's either moving the digit around, or changing a number value
const int STATE_DP = 0;
const int STATE_DG = 1;
volatile int currentState = STATE_DP;

// joystick pins, counterintuively, with my layout the x pin gives the vertical axis values, and the y pin the horizontal ones
const int xValPin = A0;
const int yValPin = A1;
const int swValPin = 3; 

// ST_CP
const int latchPin = 11;
// SH_CP
const int clockPin = 10;
// DS
const int dataPin = 12;

int digitArray[16] = {
//A B C D E F G DP 
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};


const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int displayCount = 4;

int displayDigits[displayCount] = {
  segD1, segD2, segD3, segD4
};

int digits[4]; // the digits to be displayed
volatile int dp_loc; // on which segment the decimal point should be displayed

// thresholds for moving the joystick
const int minThresh = 300;
const int maxThresh = 700;
bool joyMoved = false;

int lastSWState = HIGH;

// variables for flashing the dp
const long flashInterval = 250;
long lastFlashTime = 0;
bool flashState = LOW;

// very bizzare event, it seems like my Arduino INPUT_PULLUP is picking up noise? so it detects multiple FALLINGs on each button push
// with this variable, I will essentially rewrite FALLING
volatile bool fallen = LOW;


void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(xValPin, INPUT);
  pinMode(yValPin, INPUT);
  pinMode(swValPin, INPUT_PULLUP);
  
  for(int i = 0; i < displayCount; ++i){
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  /**
  * Read from the EEPROM. I have written 0 on the first 5 addresses, before doing any debugging, so that
  * the read always works as intended.
  */
  for(int i = 0; i < 4; ++i)
  {
    int digit = EEPROM.read(i);
    digits[i] = digit;
  }
  dp_loc = EEPROM.read(4);

  attachInterrupt(digitalPinToInterrupt(swValPin), switchToDecimal, LOW);
  Serial.begin(9600);
}


void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}


void turnOnSeg(int digit) {
  for(int i = 0; i < 4; ++i) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[digit], LOW);
} 

void showDigits(bool flash) {
  for(int i = 0; i < 4; ++i) {
    int currentDigit = digits[i];
    turnOnSeg(3 - i); 
    if(dp_loc != i) {
      writeReg(digitArray[currentDigit]);
    }
    else {
      if(flash || currentState != STATE_DP) {
        writeReg(digitArray[currentDigit] | 1);
      }
      else {
        writeReg(digitArray[currentDigit]);
      }
    }
    delay(3);
  }
}

void switchToDecimal() {
  if(!fallen) {
    if(currentState != STATE_DP) {
      currentState = STATE_DP;
    }
    else {
      currentState = STATE_DG;
      saveData();
    }
    fallen = HIGH;
  }
  else {
    showDigits(flashState);
  }
}

void saveData() {
  for(int i = 0; i < 4; ++i) {
    EEPROM.write(i, digits[i]);
  }
  EEPROM.write(4, dp_loc);
}

void handleYMovement() {
  int yVal = analogRead(yValPin);
  if(yVal > maxThresh && !joyMoved) {
    dp_loc += 1;
    dp_loc %= 4;     
    joyMoved = true;
  }
  else if(yVal < minThresh && !joyMoved) {
      dp_loc -= 1;
      if(dp_loc < 0) {
        dp_loc = 3;
      }
      joyMoved = true;
  }

  if(yVal >= minThresh && yVal <= maxThresh && joyMoved) {
    joyMoved = false;
  }
}

void handleXMovement() {
  int xVal = analogRead(xValPin);
  if(xVal > maxThresh && !joyMoved) {
    digits[dp_loc] += 1;
    digits[dp_loc] %= 10;     
    joyMoved = true;
    saveData();
  }
  else if(xVal < minThresh && !joyMoved) {
      digits[dp_loc] -= 1;
      if(digits[dp_loc] < 0) {
        digits[dp_loc] = 9;
      }
      joyMoved = true;
      saveData();
  }

  
  if(xVal >= minThresh && xVal <= maxThresh && joyMoved) {
    joyMoved = false;
  }
}

void loop() {
  int swVal = digitalRead(swValPin);
  if(swVal == HIGH) {
    fallen = LOW;
  }

  // put your main code here, to run repeatedly:
  showDigits(flashState);
  if(currentState == STATE_DP) {
    if(millis() - lastFlashTime >= flashInterval) {
      flashState = !flashState;
      lastFlashTime = millis();
    }

    // handle joystick movement
    handleYMovement();
  }
  else {
    handleXMovement();
  }
}
