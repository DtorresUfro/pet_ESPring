#include <SPI.h>
#include <MFRC522.h>
#include <HX711.h>

#define SS_PIN 21
#define RST_PIN 22
MFRC522 rfid(SS_PIN, RST_PIN);

#define BOMBA_EN 25
#define BOMBA_IN 26

#define HX_DT 4
#define HX_SCK 27
HX711 balanza;
int pesoIdeal = 0;

#define XKC 32

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
int llenarAgua() {
  int pesoInicial = obtenerPeso();
  int pesoActual = pesoInicial;
  Serial.println("Llenando agua...");

  digitalWrite(BOMBA_EN, HIGH);
  digitalWrite(BOMBA_IN, HIGH);
  
  while (pesoActual < pesoIdeal) {
    delay(200);
    pesoActual = obtenerPeso();
    Serial.print("Peso actual: ");
    Serial.println(pesoActual);
  }

  digitalWrite(BOMBA_EN, LOW);
  digitalWrite(BOMBA_IN, LOW);
  Serial.println("Bomba apagada");

  int aguaUsada = pesoActual - pesoInicial;
  if (aguaUsada < 0) { aguaUsada = 0; }
  return aguaUsada;
}

void setup() {
  Serial.begin(9600);

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
}

void loop() {
  while(digitalRead(XKC) == LOW) {
    Serial.println("Acerque una tarjeta");
    // Detectar tarjeta
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) { return; }
    Serial.println("Tarjeta detectada");
    mostrarUID();

    int aguaUsada = llenarAgua();
    Serial.print("Agua usada: ");
    Serial.println(aguaUsada);

    // Detener comunicación con tarjeta
    rfid.PICC_HaltA();

    delay(1000);
  }
  if(digitalRead(XKC) == HIGH) {
    Serial.println("No hay agua en el tanque.");
    delay(5000);
  }
}
