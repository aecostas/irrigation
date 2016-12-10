const byte IRRIGATION = 0;
const byte SLEEP = 1;
byte state = SLEEP;

const byte ledPin = 13;
const byte buttonPin = 2;
const int irrigationTime = 1000;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(ledPin, LOW);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handler, RISING);
}

/**
 * Set irrigation pin to HIGH during the specific duration
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
   state = SLEEP;
}


void handler() {
  if (IRRIGATION == state) {
      return;
  } else {
      state = IRRIGATION;   
  }

}

void loop() {
     if (IRRIGATION == state) {
         irrigate(ledPin, irrigationTime); 
     }
  
}
