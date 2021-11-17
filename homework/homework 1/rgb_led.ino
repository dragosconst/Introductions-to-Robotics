const int redValue = 0;
const int greenValue = 1;
const int blueValue = 2;

const int potRedPin = A0;
const int ledRedPin = 6;

const int potGreenPin = A2;
const int ledGreenPin = 5;

const int potBluePin = A5;
const int ledBluePin = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(potRedPin, INPUT);
  pinMode(potGreenPin, INPUT);
  pinMode(potBluePin, INPUT);
  
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);

  
  Serial.begin(9600);
}

int readPot(int which) {
  if (which == redValue) {
    return analogRead(potRedPin);
  }
  else if (which == greenValue) {
    return analogRead(potGreenPin);
  }
  else {
    return analogRead(potBluePin);
  }
}

void writeLed(int which, int analogValue) {
  int convertedValue = map(analogValue, 0, 1023, 0, 255); // analogValue is the raw val given by the potentiometer
  if(which == redValue) {
    analogWrite(ledRedPin, convertedValue); // maybe lower red values by default? red is way too strong, compared to the others
  }
  else if (which == greenValue) {
    analogWrite(ledGreenPin, convertedValue);
  }
  else {
    analogWrite(ledBluePin, convertedValue);
  }
}

void handleColor(int which) {
    writeLed(which, readPot(which));
}

void loop() {
  // put your main code here, to run repeatedly:
    handleColor(redValue);
    handleColor(greenValue);
    handleColor(blueValue);
}
