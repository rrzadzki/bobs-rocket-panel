#include <KerbalSimpit.h>

KerbalSimpit mySimpit(Serial);

int latchPin = 7,
    clockPin = 15,
    dataPin  = 11;

long lastShiftTime;

bool blinkState = false;
long blinkStartTime;
int blinkInterval = 1000;

byte leds = 0b11111111;

int ledPin = 16;

long startMillis;

void setup() {
  pinMode(ledPin, OUTPUT);
  blink(1000);
  
  Serial.begin(115200);
  while (!mySimpit.init());

  /* Test the SIPO */
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  lastShiftTime = millis();
  updateShiftRegister();
  /* end of SIPO test */

  mySimpit.inboundHandler(callbackHandler);

  mySimpit.registerChannel(SF_MESSAGE);

  startMillis = millis();
}

/**
 * Handle messages from KSP (invoked by KerbalSimPit)
 */
void callbackHandler(byte messageType, byte message[], byte messageSize) {
  blink(1000);
  switch(messageType) {
    case SCENE_CHANGE_MESSAGE:
      //blink();
      break;
    case SF_MESSAGE:
      resourceMessage rm = parseResource(message);
      float percent = rm.available / rm.total;
      updateGauge(percent);
      break;
    default:
      // blink the trouble light or something
  }
}

/**
 * Temporary: blink a status light for _interval_ millis
 */
void blink(int interval) {
  blinkState = true;
  digitalWrite(ledPin, HIGH);
  blinkStartTime = millis();
  blinkInterval = interval;
}


/**
 * Old-fashioned method to update 8 LEDs wired to the first shift register
 */
void updateGauge(float percent) {
  leds = 0;
  if(percent > 0) {
    for(int i=0; i<8; i++) {
      float pos = (float)i / 7.0;
      if(pos <= percent) bitSet(leds,i);
    }
  }
  updateShiftRegister();
}

/**
 * Shunt byte _leds_ into the register
 * 
 */
void updateShiftRegister() {
  digitalWrite(latchPin, LOW);
  // TODO: start a loop here on the _gaugeBytes_ array
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  //
  digitalWrite(latchPin, HIGH);
}

/**
 * Update a 12-segment bar graph stored in the _gaugeBytes_ array
 */

void loop() {
  mySimpit.update();
  
  if(blinkState && millis() >= blinkStartTime + blinkInterval) {
    blinkState = false;
    digitalWrite(ledPin,LOW);
  }
}
