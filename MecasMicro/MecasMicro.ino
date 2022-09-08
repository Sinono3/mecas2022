#include <Firmata.h>

#define PIN_TRIG 2
#define PIN_ECHO 3
#define PIN_PH A0
#define DISTANCE_SAMPLE_COUNT 20

int analogInputsToReport = 0;

unsigned long distanceMillis, phMillis, firmataMillis;

float distanceSamples[DISTANCE_SAMPLE_COUNT];
float ph;

void reportAnalogCallback(byte pin, int value) {
  if (value == 0) {
    analogInputsToReport = analogInputsToReport & ~(1 << pin);
  } else {  // everything but 0 enables reporting of that pin
    analogInputsToReport = analogInputsToReport | (1 << pin);
  }
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_PH, INPUT);

  Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
  Firmata.attach(REPORT_ANALOG, reportAnalogCallback);
  Firmata.begin(57600);
}

void storeDistanceSample(float ph) {
  for (int i = 0; i < DISTANCE_SAMPLE_COUNT - 1; i++) {
    distanceSamples[i] = distanceSamples[i + 1];
  }

  distanceSamples[DISTANCE_SAMPLE_COUNT - 1] = ph;
}


float getDistanceAverage() {
  float sum = 0.0;

  for (int i = 0; i < DISTANCE_SAMPLE_COUNT; i++) {
    sum += distanceSamples[i];
  }

  return sum / DISTANCE_SAMPLE_COUNT;
}

void loop() {
  if (millis() - distanceMillis >= 100) {  // Take 10 reading per second.
    distanceMillis = millis();
    // Clear the PIN_TRIG by setting it LOW:
    digitalWrite(PIN_TRIG, LOW);

    delayMicroseconds(5);

    // Trigger the sensor by setting the PIN_TRIG high for 10 microseconds:
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);

    // Read the PIN_ECHO. pulseIn() returns the duration (length of the pulse) in microseconds:
    float duration = pulseIn(PIN_ECHO, HIGH);

    // Calculate the distance:
    float distance = duration * 0.034 / 2;
    storeDistanceSample(distance);
  }

  if (millis() - phMillis >= 1000) {  // Take 1 reading per second.
    phMillis = millis();
    ph = analogRead(PIN_PH);  // Read and reverse the analogue input value from the pH sensor then scale 0-14.
  }

  while (Firmata.available())
    Firmata.processInput();

  if (millis() - firmataMillis > 20) {
    firmataMillis = millis();

    for (int analogPin = 0; analogPin < TOTAL_ANALOG_PINS; analogPin++) {
      if (analogInputsToReport & (1 << analogPin)) {
        switch (analogPin) {
          case 0:
            Firmata.sendAnalog(analogPin, ph);
            break;
          case 1:
            float distance = getDistanceAverage();
            Firmata.sendAnalog(analogPin, distance);
            break;
        }
      }
    }
  }
}
