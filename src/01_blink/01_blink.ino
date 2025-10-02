#define LED 6
#define DELAY 1000

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(DELAY);
  digitalWrite(LED, LOW);
  delay(DELAY);
}
