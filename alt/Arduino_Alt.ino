#include "HX711.h"

#define DT 4
#define SCK 5
#define PUMP_PIN 8

HX711 scale;

float pesoInicial = 0;
float pesoActual = 0;
float objetivoML = 120.0;

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
    String mensaje = Serial.readStringUntil('\n');
    int separador = mensaje.indexOf(',');
    String uid = mensaje.substring(0, separador);
    int agua = mensaje.substring(separador + 1).toInt();
    //Verificar tanque
    if (agua == 1) {
      pesoInicial = scale.get_units(5);
      digitalWrite(PUMP_PIN, HIGH);
      while (true) {
        pesoActual = scale.get_units(5);
        float diferencia = pesoActual - pesoInicial;
        //Aproxime: 1 gramo ≈ 1 ml
        if (diferencia >= objetivoML) {
          digitalWrite(PUMP_PIN, LOW);
          Serial.println((int)diferencia);
          break;
        }
        delay(100);
      }
    }
    else {
      Serial.println(0);
    }
  }
}
