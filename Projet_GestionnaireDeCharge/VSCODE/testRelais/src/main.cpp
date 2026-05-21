#include <Arduino.h>

#define RELAIS 27


void setup() {
  pinMode(RELAIS, OUTPUT);

}

void loop() {

  digitalWrite(RELAIS, LOW);
  delay(5000);
  digitalWrite(RELAIS, HIGH);
  delay(5000);

}
