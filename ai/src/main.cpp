#include <Arduino.h>

#define LED_A 2
#define LED_B 4
#define INPUT_A 22
#define INPUT_B 23

void setup() {
  Serial.begin(9600);

  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(INPUT_A, INPUT);
  pinMode(INPUT_B, INPUT);
}

void loop() {
  digitalWrite(LED_B, digitalRead(INPUT_A) ^ digitalRead(INPUT_B));
  delay(250);
}
