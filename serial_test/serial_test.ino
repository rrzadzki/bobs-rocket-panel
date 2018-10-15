byte state[2];

const int PIN_SWITCH1 = 2;

int switches[5] = {
  2,4,7,8,9
};

void setup() {
  Serial.begin(9600);
  
  for(int s=0; s<sizeof(switches)/sizeof(int); s++) {
    pinMode(switches[s],INPUT_PULLUP);
  }

  // Initialize state to 0
  for(int n=0; n<sizeof(state)-1; n++) {
    state[n] = 0;
  }
  // Set the last byte to our delimiter
  state[sizeof(state)-1] = 0xff;
}

void loop() {
  for(int s=0; s<sizeof(switches)/sizeof(int); s++) {
    int b = digitalRead(switches[s]) == LOW ? 1 : 0;
    bitWrite(state[s/8],s%8,b);
  }
  
  // put your main code here, to run repeatedly:
  for(int n=0; n<sizeof(state); n++) {
    Serial.write(state[n]);
  }
}
