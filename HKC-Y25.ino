#define XKC 32

void setup() {
  Serial.begin(9600);
  pinMode(XKC, INPUT_PULLUP);
  Serial.println("Probando XKC...");
}

void loop() {
  int estado = digitalRead(XKC);

  if (estado == HIGH) {
    Serial.println("HIGH");
  } else {
  Serial.println("LOW");
  }
  delay(500);
}