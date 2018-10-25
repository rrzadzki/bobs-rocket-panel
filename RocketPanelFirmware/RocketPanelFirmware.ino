#include <LiquidCrystal.h>

const int pisoLatchPin = 8;
const int pisoDataPin  = 9;
const int pisoClockPin = 7;

const int debugModePin = 2;

const int pisoChipCount = 1;

byte pisoState[pisoChipCount]; // TODO: Revise this math, it's wasting space.
byte pisoStateTemp[pisoChipCount];

int counter = 0;

byte heldByte = 0;
boolean holdingByte = false;

bool DEBUG_MODE = false; // TODO: allow this to be set on power-up.

LiquidCrystal lcd(10,11,3,4,5,6);

void setup() {
  pinMode(debugModePin, INPUT_PULLUP);
  if(digitalRead(debugModePin) == LOW) {
    DEBUG_MODE = true;
  }
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.print("Starting up...");

  Serial.begin(9600);
  pinMode(pisoLatchPin, OUTPUT);
  pinMode(pisoClockPin, OUTPUT);
  pinMode(pisoDataPin,  INPUT );
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("BEEP BOOP!");

  /* Send a test packet.
   *  Using the format defined in switchEvent(int ID, byte state),
   *  this will look like the following:
   *          0       1       000011
   *        | type  | state | ID     |
   *  
   *  This byte is equal to 0d67 or ASCII "C".
   */
  if(DEBUG_MODE) {
    Serial.print("Test boolean switch packet (ID 3, state 1, expect ASCII \"C\"): ");
    Serial.write(switchEvent(3,1));
    Serial.println();
  }
}

void loop() {
  /* Read in switch data from PISO */

  // Pulse the latch pin high to gather inputs, then low
  // to enter serial transmit mode.
  digitalWrite(pisoLatchPin,HIGH);
  // Wait a moment to let all those inputs stick.
  delayMicroseconds(20);
  digitalWrite(pisoLatchPin,LOW);

  // Update pisoStateTemp with the shiftIn function.
  shiftIn();

  // Compare pisoState with pisoStateTemp to decide
  // which events to send to host.
  for(int i=0; i<sizeof(pisoState)/sizeof(byte); i++) {
    // Only if a byte doesn't match will we check its bits.
    if(pisoStateTemp[i/8] != pisoState [i/8]) {
      // Examine bits to see what's different, send out
      // relevant events as we find them.
      for(int b=0; b<8; b++) {
        /* What up
         *  Bytes seem to be writing to the pisoStateTemp array
         *  as expected. They're not being read/addressed as
         *  expected here, though; check all this action.
         */
        byte state = (pisoState[i] & (1 << b)) >> b;
        byte tempState = (pisoStateTemp[i] & (1 << b)) >> b;
        int address = i*8+b;
        if(tempState != state) {
          // Ah ha, the offending bit!
          // Build an event packet and send it.
          byte packet = switchEvent(address,tempState);
          if(DEBUG_MODE) {
            Serial.print("Switch number ");
            Serial.print(address);
            Serial.print(" has state ");
            Serial.print(tempState);
            Serial.print(" which differs from stored state ");
            Serial.print(state);
            Serial.print(". Packet: ");
            Serial.print(packet, BIN);
            Serial.println();
          } else {
            Serial.write(packet);
          }
        }
      }
      // Now that all the bits have been examined, transfer temp to state.
      pisoState[i] = pisoStateTemp[i];
    }
    if(DEBUG_MODE) Serial.println("--End of byte--");
  }
  if(DEBUG_MODE) Serial.println("---End of state---");

  /* Receive any bytes from serial */
  while(Serial.available() > 0) {
    byte b = Serial.read();
    
    // Check if we're holding over a byte from the last loop.
    if(holdingByte) {
      holdingByte = false;
      if(b & 0b10000000 == 0) {
        byte id    = heldByte & 0b00111111;
        byte value =        b & 0b01111111;
        setGauge(id,value);
      } else {
        // TODO: this is an error, some kind of malformed message
      }
    }
    
    // Check if this byte "applies to next"
    if((b & 0b10000000) == 0) {
      // This is a standalone message.
      // Bit 2^6 is the new state.
      // Bits 2^5 through 2^0 are the gauge ID.
      boolean state = b & 0b01000000;
      byte id       = b & 0b00111111;
      setIndicator(id,state);
    } else {
      // This is a routing byte, indicating where we'll assign the next byte's value.
      // If there are more bytes available, grab one.
      if(Serial.available() > 0) {
        byte c = Serial.read();
        if(c & 0b10000000 == 0) {
          byte id    = b & 0b00111111;
          byte value = c & 0b01111111;
          setGauge(id,value);
        } else {
          // TODO: this is an error, some kind of malformed message
        }
      } else {
        // Since there are no more bytes available right now, store this
        // byte and set the flag indicating we'll get to it next time.
        heldByte = b;
        holdingByte = true;
      }
    }
  }
  
  if(DEBUG_MODE) {
    lcd.setCursor(0,1);
    lcd.print(counter++);
  }
  
  if(DEBUG_MODE) delay(1000);
}

void shiftIn() {
  /* Pulse the clock once per bit, assembling an output byte. */
  byte temp;
  // With these 2 lines, the FIRST byte is read correctly.
  // Without them EVERY byte displays the mysterious LSB 1.
  digitalWrite(pisoClockPin,HIGH);
  delayMicroseconds(0.2);
  /* Read chips * 7 since there are 7 switches and 7 bits per chip. */
  for(int i=0; i<pisoChipCount*7; i++) {
    /* Pack results into 8-bit bytes in the temporary state array. */
    if(i%8==0) pisoStateTemp[i/8] = 0;
    digitalWrite(pisoClockPin, LOW);
    delayMicroseconds(0.2);
    temp = digitalRead(pisoDataPin);
    if(temp) {
      // The bit is high; set it so in the state array.
      pisoStateTemp[i/8] = pisoStateTemp[i/8] | (1 << (i%8));
    }
    /* Debug: Send the bits to serial as it comes in. */
    /*
    Serial.print(i);
    Serial.print(" Bit: ");
    Serial.println(temp, BIN);
    */
    if(DEBUG_MODE) {
      Serial.print("pisoStateTemp byte=");
      Serial.print(pisoStateTemp[i/8],BIN);
      Serial.println();
    }
    digitalWrite(pisoClockPin,HIGH);
  }
  digitalWrite(pisoClockPin,LOW);
  /* Debug: Send the whole byte over serial. */
  /*
  Serial.println(pisoStateTemp[0],BIN);
  Serial.println("------");
  */
}

byte switchEvent(int id, boolean state) {
  // TODO: Document this function
  byte packet = id;
  bitWrite(packet,7,0); // MSB is for "applies to next ID"; bools do not.
  bitWrite(packet,6,state);
  return packet;
}

void setIndicator(int id, boolean state) {
  // TODO
}

void setGauge(int id, int value) {
  // TODO
}

