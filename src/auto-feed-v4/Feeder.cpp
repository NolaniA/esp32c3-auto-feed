#include "Feeder.h"

#include <ESP32Servo.h>

namespace {
Servo feederServo;
}

void setupFeeder(int servoPin) {
  feederServo.attach(servoPin);
  feederServo.writeMicroseconds(1500);
}

void doFeed(int durationSec) {
  Serial.printf("FEED START %d sec\n", durationSec);

  feederServo.writeMicroseconds(2000);
  delay(durationSec * 1000UL);
  feederServo.writeMicroseconds(1500);

  Serial.println("FEED DONE");
}
