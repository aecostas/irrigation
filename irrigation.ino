#define DEBUG

#ifdef DEBUG
 #define DEBUG_PRINT(x)     Serial.print (x)
 #define DEBUG_PRINTDEC(x)     Serial.print (x, DEC)
 #define DEBUG_PRINTLN(x)  Serial.println (x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTDEC(x)
 #define DEBUG_PRINTLN(x) 
#endif

const byte pumpPin = 13;    // substitute by peristaltic bump pin
const byte buttonPin = 2;

const int irrigationTime = 3000;
const int holdTime = 1500;

const byte STATUS_SLEEP = 0;
const byte STATUS_IRRIGATING = 1;
const byte STATUS_IRRIGATING_MANUAL = 2;
const byte STATUS_HOLD = 3;

const byte ACTION_NONE = 0;
const byte ACTION_BUTTON_PRESS = 1;
const byte ACTION_BUTTON_RELEASE = 2;

unsigned long last_irrigation_time = 0;
unsigned long start_irrigation_time = 0;
unsigned long buttonFalling = 0;

int timeout = 60000;
byte state = STATUS_SLEEP;
byte action = ACTION_NONE;

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  pinMode(pumpPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(pumpPin, LOW);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handler_buttonPin_change, CHANGE);
}

/**
 * Set irrigation pin to HIGH during the specific duration.
 * (blocking function)
 * 
 * @param pin - irrigation pin
 * @param duration - duration of the irrigation (millis)
 */
void start_irrigation(byte pin) {
   digitalWrite(pin, HIGH);   
   start_irrigation_time = millis();
}

void end_irrigation(byte pin) {
   digitalWrite(pin, LOW);
   last_irrigation_time = millis();
}

void handler_buttonPin_change() {
    if (digitalRead(buttonPin) == LOW) {
       action = ACTION_BUTTON_PRESS;
       buttonFalling = millis(); 
    } else {
       action = ACTION_BUTTON_RELEASE;  
    }
}

void loop() {
  unsigned long now = millis(); 
  // TODO: hacer diagrama de estado incluyendo el estado
  // pulsado el boton de regado

  switch (state) {
    case STATUS_SLEEP:
         DEBUG_PRINTLN("STATUS_SLEEP");

         if (action == ACTION_BUTTON_PRESS) {
             action = ACTION_NONE;
             state = STATUS_HOLD; 
         }
         if ((now - last_irrigation_time) >= timeout) {
           start_irrigation(pumpPin);
           state = STATUS_IRRIGATING;
         }

       break;

    case STATUS_IRRIGATING:
         DEBUG_PRINTLN("STATUS_IRRIGATING");

         if ((now - start_irrigation_time) >= irrigationTime) {
            end_irrigation(pumpPin);
            state = STATUS_SLEEP;
         }
       break;

    case STATUS_IRRIGATING_MANUAL:
       DEBUG_PRINTLN("STATUS_IRRIGATING_MANUAL");
       if (action == ACTION_BUTTON_RELEASE) {
         action = ACTION_NONE;
         end_irrigation(pumpPin);
         state = STATUS_SLEEP;
       }
       break;

    case STATUS_HOLD:
       DEBUG_PRINTLN("STATUS_HOLD");
       if (action == ACTION_BUTTON_RELEASE) {
             action = ACTION_NONE;
             start_irrigation(pumpPin);
             state = STATUS_IRRIGATING;
       } 
       else if ((now - buttonFalling) >= holdTime) {
           DEBUG_PRINTLN("starting irrigation from hold");
           start_irrigation(pumpPin);
           state = STATUS_IRRIGATING_MANUAL;
       }
       break;
  }

}

