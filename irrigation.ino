const byte IRRIGATION = 0;
const byte SLEEP = 1;

const byte ledPin = 13;    // substitute by peristaltic bump pin
const byte buttonPin = 2;
const int irrigationTime = 1000;

unsigned long lastIrrigation = 0;
int timeout = 5000;
byte state = SLEEP;

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
   lastIrrigation = millis();
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

  // irrigate after pressing the button
  // or on timeout from last irrigation
  
     if (IRRIGATION == state) {
         irrigate(ledPin, irrigationTime); 
     }  

     if (millis() - lastIrrigation >= timeout) {
         irrigate(ledPin, irrigationTime);
     }

}



