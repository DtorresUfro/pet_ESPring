#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";

String apiKey = "TU_API_KEY";
//RFID
#define SS_PIN 5
#define RST_PIN 22
MFRC522 rfid(SS_PIN, RST_PIN);
//XKC-Y25
#define WATER_SENSOR 15

HardwareSerial ArduinoSerial(2);

String uidString = "";
bool aguaDisponible = false;

void setup() {
  Serial.begin(115200);
  ArduinoSerial.begin(9600, SERIAL_8N1, 16, 17);
  pinMode(WATER_SENSOR, INPUT);
  SPI.begin();
  rfid.PCD_Init();
  WiFi.begin(ssid, password);
}

void loop() {
  aguaDisponible = digitalRead(WATER_SENSOR);
  if (rfid.PICC_IsNewCardPresent() &&
      rfid.PICC_ReadCardSerial()) {
    uidString = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uidString += String(rfid.uid.uidByte[i], HEX);
    }
    // Enviar al Arduino
    String mensaje = uidString + "," + String(aguaDisponible);
    ArduinoSerial.println(mensaje);
    Serial.println("Enviado:");
    Serial.println(mensaje);
    // Esperar respuesta del Arduino
    String mlUsados = "";
    unsigned long inicio = millis();
    while (millis() - inicio < 10000) {
      if (ArduinoSerial.available()) {
        mlUsados = ArduinoSerial.readStringUntil('\n');
        enviarThingSpeak(uidString, aguaDisponible, mlUsados.toInt());
        break;
      }
    }
    rfid.PICC_HaltA();
  }
  delay(500);
}

void enviarThingSpeak(String uid, bool agua, int ml) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url =
      "http://api.thingspeak.com/update?api_key=" + apiKey +
      "&field1=" + uid +
      "&field2=" + String(agua) +
      "&field3=" + String(ml);
    http.begin(url);
    int httpCode = http.GET();
    Serial.println(httpCode);
    http.end();
  }
}
