byte state[2];

/* 64 boolean switches MAX */
int switches[5] = {
  2,4,7,8,9
};

bool previousStates[5];
bool states[5] = {0,0,0,0,0};

unsigned long debounceTimes[5];
unsigned long debounceDelay = 50;

unsigned long analog_state = 0;
unsigned long last_analog_broadcast;

unsigned char beep = 0;

void setup() {
  Serial.begin(9600);
  
  for(int s=0; s<sizeof(switches)/sizeof(int); s++) {
    pinMode(switches[s],INPUT_PULLUP);
  }

  last_analog_broadcast = millis();

  /* Send a test packet.
   *  Using the format defined in switchEvent(int ID, byte state),
   *  this will look like the following:
   *          0       1       000011
   *        | type  | state | ID     |
   *  
   *  This byte is equal to 0d67 or ASCII "C".
   */
  Serial.print("Test boolean switch packet (ID 3, state 1): ");
  unsigned char packet = 3;
  bitWrite(packet,7,0); // MSB for "applies to next ID"
  bitWrite(packet,6,1); // Value in 2^6 bit
  Serial.write(packet);
  Serial.println();

  /*
  // Initialize state to 0
  for(int n=0; n<sizeof(state)-1; n++) {
    state[n] = 0;
  }
  // Set the last byte to our delimiter
  state[sizeof(state)-1] = 0xff;
  */
}

void loop() {
  // Search switch values for changes
  for(int s=0; s<sizeof(switches)/sizeof(int); s++) {
    bool b = digitalRead(switches[s]) == LOW ? 1 : 0;
    // Debounce
    if(previousStates[s] != b) {
      debounceTimes[s] = millis();
    }
    if(millis() - debounceTimes[s] > debounceDelay) {
      if(states[s] != b) {
        // Send this event and then set states[s] to b
        unsigned char packet = s;
        bitWrite(packet,7,0); // MSB for "applies to next ID"
        bitWrite(packet,6,b); // Value in 2^6 bit
        Serial.write(packet);
        states[s] = b;
      }
    }
    previousStates[s] = b;
  }

  // Send analog values
  if(millis() - last_analog_broadcast > 50) { // chatter was driving me nuts while debugging; needs a cap!
    last_analog_broadcast = millis();
    unsigned long analog_reading = analogRead(A0);
    analog_reading = analog_reading >> 3;
    if(analog_reading != analog_state) {
      unsigned char msg[2] = {analog_reading | 0b10000000,31};
      Serial.write(msg,2);
      analog_state = analog_reading;
      analogWrite(11,analog_reading);
    }
  }
  
}
