#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);
HardwareSerial ArduinoSerial(2);

const char* ssid = "nombre_wifi";
const char* password = "contraseña";

String apiKey = "key_ts";
String uidString = "";

void setup() {
  Serial.begin(115200);
  ArduinoSerial.begin(9600, SERIAL_8N1, 16, 17);
  SPI.begin();
  rfid.PCD_Init();
  WiFi.begin(ssid, password);
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() &&
      rfid.PICC_ReadCardSerial()) {
    uidString = "";

    for (byte i = 0; i < rfid.uid.size; i++) {
      uidString += String(rfid.uid.uidByte[i], HEX);
    }
    Serial.println("Mascota detectada");
    // Enviar comando al Arduino
    ArduinoSerial.println("FILL");
    esperarConfirmacion();
    enviarThingSpeak();
    rfid.PICC_HaltA();
  }
}

void esperarConfirmacion() {
  while (true) {
    if (ArduinoSerial.available()) {
      String msg = ArduinoSerial.readStringUntil('\n');
      msg.trim();
      if (msg == "DONE") {
        Serial.println("Llenado completo");
        break;
      }
    }
  }
}

void enviarThingSpeak() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url =
      "http://api.thingspeak.com/update?api_key=" +
      apiKey +
      "&field1=" + uidString;
    http.begin(url);
    int httpCode = http.GET();
    Serial.println(httpCode);
    http.end();
  }
}