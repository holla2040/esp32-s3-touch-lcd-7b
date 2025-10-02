#include <ctype.h>

#define DELAY 1000
#define LED 6



unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  Serial0.begin(115200);
  pinMode(LED, OUTPUT);
  delay(2000);
  Serial.println("03_serial setup");
  Serial0.println("Serial1 TX test");
}

void loop() {
  char c;
  if (Serial.available()) {
    c = Serial.read();
    Serial0.print(c);
    Serial.print(c); //local echo
  }
  if (Serial0.available()) {
    c = Serial0.read();
    c = toupper(c);
    Serial.print(c);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= DELAY) {
    previousMillis = currentMillis;
    digitalWrite(LED, !digitalRead(LED));
  }
}

