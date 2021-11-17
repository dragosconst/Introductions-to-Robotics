const int sampleSize = 500;


const int pinBuzz = 10;

int baseFrequency = 500; // frequency for a detection of 5
long lastBuzz = 0;
long buzzStep = 100; // the step by which it gets faster, for every step above 0 on the display
long normalInterval = 1000; // the interval for buzzes at a value of 0 on the 7-segment display
long playInterval = 20; // how long it should play the sound
bool screechState = LOW;

int freqStep = 50; // step it adds\decreases depending on how far from 5 it is


const int pinGhost = A0;

const int pinA = 2;
const int pinB = 3;
const int pinC = 4;
const int pinD = 5;
const int pinE = 6;
const int pinF = 7;
const int pinG = 8;
const int pinDP = 9;

const int segSize = 8;
const int noOfDigits = 10;
bool dpState = LOW;


bool digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

void displayNumber(byte digit, bool decimalPoint) {
  for(int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  digitalWrite(pinDP, decimalPoint);
}

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < 8; ++i) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinBuzz, OUTPUT);
  pinMode(pinGhost, INPUT);
  

  Serial.begin(9600);
}

void playAtIntervals(int score) {
  if(!screechState && millis() - lastBuzz >= normalInterval - score * buzzStep) {
    lastBuzz = millis();
    screechState = HIGH;
    tone(pinBuzz, baseFrequency + score * freqStep);
  }
  else if(screechState && millis() - lastBuzz < playInterval){
    tone(pinBuzz, baseFrequency + score * freqStep);
  }
  else if(screechState && millis() - lastBuzz >= playInterval) {
    noTone(pinBuzz);
    lastBuzz = millis();
    screechState = LOW;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  long inputsSummed = 0;
  for(int i = 0; i < sampleSize; ++i) {
    int currentInput = analogRead(pinGhost);
    inputsSummed += currentInput;
  }

  int averageInput = inputsSummed / sampleSize;
  averageInput = constrain(averageInput, 0, 100);
  averageInput = map(averageInput, 0, 100, 0, 9);

  playAtIntervals(averageInput);
  
  displayNumber(averageInput, false);
}
