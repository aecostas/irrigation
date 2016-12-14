const byte IRRIGATION = 0;
const byte SLEEP = 1;

const byte pumpPin = 13;    // substitute by peristaltic bump pin
const byte buttonPin = 2;
const int irrigationTime = 1000;

unsigned long lastIrrigation = 0;
unsigned long buttonFalling = 0;

int timeout = 5000;
byte state = SLEEP;

void setup() {
  Serial.begin(9600);
  pinMode(pumpPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(pumpPin, LOW);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handler_rising, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handler_falling, FALLING);
}

/**
 * Set irrigation pin to HIGH during the specific duration.
 * (blocking function)
 * 
 * @param pin - irrigation pin
 * @param duration - duration of the irrigation (millis)
 */
void irrigate(byte pin,int duration) {
   state = IRRIGATION;
   Serial.print("Start irrigation...\n");

   digitalWrite(pin, HIGH);
   delay(duration);
   digitalWrite(pin, LOW);  
   
   Serial.print("End irrigation\n");
   lastIrrigation = millis();
   state = SLEEP;
}

void handler_falling() {
    buttonFalling = millis();
}


void handler_rising() {
  if (IRRIGATION == state) {
    return;
  } else {
    state = IRRIGATION; 
  }
}

void loop() {
  // TODO: hacer diagrama de estado incluyendo el estado
  // pulsado el boton de regado


  // irrigate after pressing the button
  // or on timeout from last irrigation
  
  if (IRRIGATION == state) {
    irrigate(pumpPin, irrigationTime); 
  }

  if (millis() - lastIrrigation >= timeout) {
    irrigate(pumpPin, irrigationTime);
  }

}



