#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);

const char* pubTopic = "channels/3361732/publish";
WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  while (!client.connected()) { //cliendID, mqttUser, mqttPassword
    if (client.connect("OAsXNyM7LDIANBsaLiUWAww", "OAsXNyM7LDIANBsaLiUWAww", "GLyT/FlCLjQRhhTtV91rVYZl")) {}
    else {delay(5000);}
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("NombreRed", "contraseña");
  client.setServer("mqtt3.thingspeak.com", 1883);
  
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if (!client.connected()) {reconnect();}
  client.loop();

  if (rfid.PICC_IsNewCardPresent()){
    String payload = "field2=1";
    client.publish(pubTopic, payload.c_str());
    Serial.print("Se notificó la presencia de la tarjeta.");
  }

  delay(20000);
}