#include "HX711.h"

#define DT 4
#define SCK 5

#define PUMP_PIN 8

HX711 scale;

float pesoObjetivo = 150.0;

void setup() {
  Serial.begin(9600);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  scale.begin(DT, SCK);
  scale.set_scale(2280.f);
  scale.tare();
}

void loop() {
  if (Serial.available()) {
    String comando =
      Serial.readStringUntil('\n');
    comando.trim();
    if (comando == "FILL") {
      llenarAgua();
    }
  }
}

void llenarAgua() {
  digitalWrite(PUMP_PIN, HIGH);
  while (true) {
    float peso = scale.get_units(5);
    Serial.print("Peso: ");
    Serial.println(peso);
    if (peso >= pesoObjetivo) {
      digitalWrite(PUMP_PIN, LOW);
      Serial.println("DONE");
      break;
    }
    delay(200);
  }
}