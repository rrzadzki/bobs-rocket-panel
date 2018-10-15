const bool TYPE_BOOLEAN = false;
const bool TYPE_SCALAR = true;

byte state[2];

/* 64 boolean switches MAX */
int switches[5] = {
  2,4,7,8,9
};

bool previousStates[5];
bool states[5] = {0,0,0,0,0};

unsigned long debounceTimes[5];
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  
  for(int s=0; s<sizeof(switches)/sizeof(int); s++) {
    pinMode(switches[s],INPUT_PULLUP);
  }

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
        bitWrite(packet,7,TYPE_BOOLEAN); // MSB for message type
        bitWrite(packet,6,b);            // Value in next bit
        Serial.write(packet);
        states[s] = b;
        /*
        Serial.write(s);
        Serial.write(b);
        Serial.write(0xff);
        */
      }
    }
    previousStates[s] = b;
  }
}
