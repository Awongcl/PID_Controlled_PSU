#include <Arduino.h>

int pwm = A0;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(A0);
  float voltage = value*(15.0/1023.0);

  //Serial.print(value);

  Serial.print(voltage);
  Serial.print("\n");
  delay(500);
}