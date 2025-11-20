#include <ESP32Servo.h>

Servo servo1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  servo1.attach(D10);
}

void loop() {
  // put your main code here, to run repeatedly:
  servo1.write(90);
  Serial.println("writing 90");
  delay(1000);
  servo1.write(0);
  Serial.println("writing 0");
  delay(1000);
  servo1.write(90);
  Serial.println("writing 90");
  delay(1000);
  servo1.write(180);
  Serial.println("writing 180");
  delay(1000);
}
