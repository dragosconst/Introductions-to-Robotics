const int stateOne = 0;
const int waitForButtonPress = 1;
const int stateTwo = 2;
const int stateThree = 3;
const int stateFour = 4; // states defined in the homework pdf + intermediary custom state used for waiting 10 seconds after a button press
// the intermediary state exists so that a button press won't reset the current wait time while we are in state 1, which would be an undesired behaviour, I think
// IRL at least I wouldn't like if spamming the button for semaphores would actually result in longer wait times

const int audioPin = 9;

const int buttonPin = 13;

const int greenCarPin = 2;
const int yellowCarPin = 4;
const int redCarPin = 7;
const int greenPedPin = 8;
const int redPedPin = 12; // I used exclusively digital pins for ease of writing code, we don't care about fade-in\out effects

int currentState = stateOne;

/**
* NOTE: I used longs because it seems like unsigned int can't hold for more than ~1 minute before overflowing, which gives the need to reset the system really often for debugging.
*/

unsigned long lastSemChange = 0;
const unsigned long pedestrianInterval = 10000; // 10 seconds between green and red for pedestrians
const unsigned long pushInterval = 10000; // 10 seconds to change light after button press
const unsigned long yellowInterval = 3000; // 3 seconds when it switches to yellow
const unsigned long flashingInterval = 5000; // 5 seconds for fast flashses

unsigned long lastFlash = 0;
const unsigned long flashInterval = 200; // interval at which the green semaphore flashes for pedestrians
bool greenState = HIGH; // the state of the pedestrian green light

const unsigned long screechInterval = 300;
const unsigned long pauseInterval = 200;
const unsigned long longScreechInterval = 900; // long screeches are for the first 3 seconds of the sempahore
const unsigned long longPauseInterval = 300;
unsigned long lastScreech = 0;
bool screechState = LOW;
const unsigned long screechFrequency = 750;

bool buttonState = HIGH;
bool previousState = HIGH;

int buttonReading = HIGH;
int previousButtonReading = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // variables for handling the button inputs

void setup() {
  // put your setup code here, to run once:
  pinMode(audioPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(greenCarPin, OUTPUT);
  pinMode(yellowCarPin, OUTPUT);
  pinMode(redCarPin, OUTPUT);
  pinMode(greenPedPin, OUTPUT);
  pinMode(redPedPin, OUTPUT);

  Serial.begin(9600);
}

bool buttonPushed(int reading, int previousReading) { // function to check if the button was pushed or not
  if(reading != previousReading) {
    lastDebounceTime = millis();
  }

  if(millis() - lastDebounceTime > debounceDelay) 
  {
    if(reading != buttonState) {
      buttonState = reading;
      if(buttonState == LOW) {
        return HIGH;
      }
    }
  }

  return LOW;
}

void handleLeds() { // handling led states, depending on the system's state
  if(currentState == stateOne || currentState == waitForButtonPress) {
    digitalWrite(redCarPin, LOW);
    digitalWrite(yellowCarPin, LOW);
    digitalWrite(greenCarPin, HIGH);

    digitalWrite(redPedPin, HIGH);
    digitalWrite(greenPedPin, LOW);
  }
  else if(currentState == stateTwo) {
    digitalWrite(redCarPin, LOW);
    digitalWrite(yellowCarPin, HIGH);
    digitalWrite(greenCarPin, LOW);

    digitalWrite(redPedPin, HIGH);
    digitalWrite(greenPedPin, LOW);
  }
  else if(currentState == stateThree) {
    digitalWrite(redCarPin, HIGH);
    digitalWrite(yellowCarPin, LOW);
    digitalWrite(greenCarPin, LOW);
    
    digitalWrite(redPedPin, LOW); 
    digitalWrite(greenPedPin, HIGH);
  }
  else if(currentState == stateFour) {
    digitalWrite(redCarPin, HIGH);
    digitalWrite(yellowCarPin, LOW);
    digitalWrite(greenCarPin, LOW);
    
    digitalWrite(redPedPin, LOW);
    if(millis() - lastFlash >= flashInterval) {
      greenState = !greenState;
      lastFlash = millis();
      digitalWrite(greenPedPin, greenState);
    }
  }
}

void makeSounds() {
  if(currentState == stateThree) { // this state will have longer screeches
    if(!screechState && millis() - lastScreech >= longScreechInterval) { // LOW screechState means it should be screeching and HIGH means it should not be
      lastScreech = millis();
      noTone(audioPin);
      screechState = !screechState;
    }
    else if (!screechState) {
      tone(audioPin, screechFrequency);
    }

    if(screechState && millis() - lastScreech >= longPauseInterval) {
      lastScreech = millis();
      tone(audioPin, screechFrequency);
      screechState = !screechState;
    }
    else if (screechState) {
      noTone(audioPin);
    }
  }
  else if(currentState == stateFour) { // shorter, more often screeches in this state
    if(!screechState && millis() - lastScreech >= screechInterval) {
      lastScreech = millis();
      noTone(audioPin);
      screechState = !screechState;
    }
    else if (!screechState) {
      tone(audioPin, screechFrequency);
    }

    if(screechState && millis() - lastScreech >= pauseInterval) {
      lastScreech = millis();
      tone(audioPin, screechFrequency);
      screechState = !screechState;
    }
    else if (screechState) {
      noTone(audioPin);
    }
  }
  else {
    noTone(audioPin); // just making sure it shuts up in any other state
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // handling state transitions
  buttonReading = digitalRead(buttonPin);
  if(buttonPushed(buttonReading, previousButtonReading) && currentState == stateOne) {
    currentState = waitForButtonPress; // intermediary state for waiting 10 seconds
    lastSemChange = millis();
  }

  if(currentState == waitForButtonPress && millis() - lastSemChange >= pushInterval) {
    currentState = stateTwo;
    lastSemChange = millis();
  }

  if(currentState == stateTwo && millis() - lastSemChange >= yellowInterval) {
    currentState = stateThree;
    lastSemChange = millis();
  }

  if(currentState == stateThree && millis() - lastSemChange >= pedestrianInterval) {
    currentState = stateFour;
    lastSemChange = millis();
  }

  if(currentState == stateFour && millis() - lastSemChange >= flashingInterval) {
    currentState = stateOne;
    lastSemChange = millis();
  }

  handleLeds();
  makeSounds();
  
  previousButtonReading = buttonReading;
}
