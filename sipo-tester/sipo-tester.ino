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

// todo: Implement leds[], maybe change to gauges[] or outpins[] or something
byte leds = 0b11111111;

int counter = 0;

int ledPin = 16;

long startMillis;

/* Gauge cycling globals
 *  Each gauge is divided into three segments. The entire bank will be
 *  cycled at once, at a rate defined here.
 */
int gauge_hz = 20;
long gauge_cycle_ms, last_gauge_cycle_time;
int gauge_segment_active = 0; // will be 0, 1, or 2
const GAUGE_SEGMENTS = 3;
const GAUGE_COUNT = 1;

byte gauge_bytes[][];

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

  gauge_bytes[][] = new byte[GAUGE_SEGMENTS][GAUGE_COUNT];

  gauge_cycle_ms = 1000 / gauge_hz;

  startMillis = 
  lastFlashTime = 
  last_gauge_cycle_time = millis();

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
  long ms = millis();
  // cycle the gauge bank
  if(ms > last_gauge_cycle_time + gauge_cycle_ms) {
    last_gauge_cycle_time = ms;

    // Set the current segment pin low
    
    gauge_segment_active = (gauge_segment_active + 1) % GAUGE_SEGMENTS;

    // Set the current segment pin high

    // Update the shiftOut register with this segment's values
    digitalWrite(latchPin, LOW);
    for(int g=0; g<GAUGE_COUNT; g++) {
      shiftOut(dataPin, clockPin, LSBFIRST, gauge_bytes[gauge_segment_active][g]);
    }
    digitalWrite(latchPin, HIGH);
  }
  
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
