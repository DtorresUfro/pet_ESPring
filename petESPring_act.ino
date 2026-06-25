#include <SPI.h>
#include <MFRC522.h>
#include <HX711.h>
#include <WiFi.h>
#include <ThingSpeak.h>

WiFiClient client;
const char* ssid = "WIFI-ESP32-1";
const char* password = "MyAwesomeSuperSecretESP32Password";
unsigned long channelID = 3382497;
const char* writeAPIKey = "BMECRSIRKAYJMOXR";

#define SS_PIN 21
#define RST_PIN 22
MFRC522 rfid(SS_PIN, RST_PIN);

#define BOMBA_EN 25
#define BOMBA_IN 26
bool estadoBomba = false;
bool ultimoEstadoBomba = false;
bool estadoTanque = false;
bool ultimoEstadoTanque = false;

#define HX_DT 4
#define HX_SCK 27
HX711 balanza;
int pesoIdeal = 0;

#define XKC 32
unsigned long ultimoEnvio = 0;

void mostrarUID() {
  Serial.print("UID: ");
    for (byte i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] < 0x10)
        {
            Serial.print("0");
        }
        Serial.print(rfid.uid.uidByte[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}
void activarBomba() {
  Serial.println("BOMBA ENCENDIDA");
  // Activar L293D
  digitalWrite(BOMBA_EN, HIGH);
  digitalWrite(BOMBA_IN, HIGH);
  delay(1000);

  Serial.println("BOMBA APAGADA");
  // Apagar
  digitalWrite(BOMBA_EN, LOW);
  digitalWrite(BOMBA_IN, LOW);
}
int obtenerPeso() {
  float lectura = balanza.get_units(5);
  int peso = (int)lectura;
  if (peso < 0) { peso = 0; }
  return peso;
}
void subirDatos(int pesoInicial, int pesoFinal, int estadoBomba, int estadoTanque) {
  ThingSpeak.setField(1, pesoInicial);
  ThingSpeak.setField(2, pesoFinal);
  ThingSpeak.setField(3, estadoBomba);
  ThingSpeak.setField(4, estadoTanque);
  int respuesta = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (respuesta == 200) {
    Serial.println("Datos enviados a ThingSpeak");
  } else {
    Serial.print("Error ThingSpeak: ");
    Serial.println(respuesta);
  }
}
void llenarAgua() {

  int pesoInicial = obtenerPeso();
  int pesoActual = pesoInicial;

  Serial.println("Llenando agua...");

  // Encender bomba
  digitalWrite(BOMBA_EN, HIGH);
  digitalWrite(BOMBA_IN, HIGH);

  // Tiempo inicial
  unsigned long inicio = millis();

  while (pesoActual < pesoIdeal) {

    // Timeout de 10 segundos
    if (millis() - inicio > 10000) {

      Serial.println("ERROR: Timeout de llenado");

      break;
    }

    delay(200);

    pesoActual = obtenerPeso();

    Serial.print("Peso actual: ");
    Serial.println(pesoActual);
  }

  // Apagar bomba
  digitalWrite(BOMBA_EN, LOW);
  digitalWrite(BOMBA_IN, LOW);

  Serial.println("Bomba apagada");

  subirDatos(
    pesoInicial,
    pesoActual,
    0,
    digitalRead(XKC) == LOW ? 1 : 0
  );
}
void verificarCambiosEstado() {

  // Leer estados actuales
  estadoBomba = digitalRead(BOMBA_EN);

  estadoTanque = (digitalRead(XKC) == LOW);

  // Detectar cambio bomba
  if (estadoBomba != ultimoEstadoBomba) {

    Serial.println("Cambio estado bomba");

    subirDatos(
      0,
      0,
      estadoBomba ? 1 : 0,
      estadoTanque ? 1 : 0
    );

    ultimoEstadoBomba = estadoBomba;
  }

  // Detectar cambio tanque
  if (estadoTanque != ultimoEstadoTanque) {

    if (millis() - ultimoEnvio > 15000) {

      subirDatos(
        0,
        0,
        estadoBomba ? 1 : 0,
        estadoTanque ? 1 : 0
      );

      ultimoEnvio = millis();
    }

    ultimoEstadoTanque = estadoTanque;
  }
}

void setup() {  
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");

  ThingSpeak.begin(client);

  delay(5000);

  balanza.begin(HX_DT, HX_SCK);
  balanza.set_scale(7050.0);
  balanza.tare();
  Serial.println("HX711 listo");

  Serial.print("Peso líquido: ");
  delay(3000);
  pesoIdeal = obtenerPeso();
  Serial.println(pesoIdeal);

  pinMode(BOMBA_EN, OUTPUT);
  pinMode(BOMBA_IN, OUTPUT);
  digitalWrite(BOMBA_EN, LOW);
  digitalWrite(BOMBA_IN, LOW);

  pinMode(XKC, INPUT_PULLUP);

  SPI.begin(18, 23, 19, 21);
  rfid.PCD_Init();
  Serial.println("RFID listo");

  ThingSpeak.setField(1, 123);
  int respuesta = ThingSpeak.writeFields(channelID, writeAPIKey);
  Serial.println(respuesta);
}

void loop() {
  verificarCambiosEstado(); //Verifica el estado del tanque.

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconectando WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi reconectado");
  } //Comprobar Wifi

  if(digitalRead(XKC) == LOW) {
    // Detectar tarjeta
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) { return; }
    Serial.println("Tarjeta detectada");
    mostrarUID();

    llenarAgua();

    Serial.println("Proceso completado");

    // Detener comunicación con tarjeta
    rfid.PICC_HaltA();

    delay(1000);
  }
  if(digitalRead(XKC) == HIGH) {
    Serial.println("No hay agua en el tanque.");
    delay(5000);
  }
}