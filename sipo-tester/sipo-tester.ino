#include <KerbalSimpit.h>

KerbalSimpit mySimpit(Serial);

int latchPin = 7,
    clockPin = 15,
    dataPin  = 11;

long lastShiftTime;

bool blinkState = false;
long blinkStartTime;
int blinkInterval = 1000;

int flashInterval = 300;
long lastFlashTime;
bool flashState = false;

byte leds = 0b11111111;

int counter = 0;

int ledPin = 16;

long startMillis;

void setup() {
  pinMode(ledPin, OUTPUT);
  blink(1000);
  
  Serial.begin(115200);

  /* Test the SIPO */
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  lastShiftTime = millis();
  updateShiftRegister();

  startMillis = lastFlashTime = millis();
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
 * Shunt byte _leds_ into the register
 */
void updateShiftRegister() {
  digitalWrite(latchPin, LOW);
  // TODO: start a loop here on the _gaugeBytes_ array
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  //
  digitalWrite(latchPin, HIGH);
}

void loop() {
  if(blinkState && millis() >= blinkStartTime + blinkInterval) {
    blinkState = false;
    digitalWrite(ledPin,LOW);
  }

  if(millis() > flashInterval + lastFlashTime) {
    lastFlashTime = millis();

    counter = (counter + 1) % 12;

    leds = (1 << 7 - counter / 4) | (1 << 4 - counter % 4);
    //(0b1 << counter % 4) | (0b1 << counter / 4);

    Serial.println(leds, BIN);
    
    updateShiftRegister();
  }
}
