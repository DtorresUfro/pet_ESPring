#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 21
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);

void setup()
{
    Serial.begin(9600);

    delay(2000);

    Serial.println("Iniciando RFID...");

    // SPI ESP32
    SPI.begin(18, 23, 19, 21);

    // Inicializar RC522
    rfid.PCD_Init();

    Serial.println("RFID listo");
    Serial.println("Acerque una tarjeta");
}

void loop()
{
    // Detectar tarjeta
    if (!rfid.PICC_IsNewCardPresent())
    {
        delay(50);
        return;
    }

    // Leer tarjeta
    if (!rfid.PICC_ReadCardSerial())
    {
        delay(50);
        return;
    }

    Serial.println("Tarjeta detectada");

    Serial.print("UID: ");

    for (byte i = 0; i < rfid.uid.size; i++)
    {
        if (rfid.uid.uidByte[i] < 0x10)
        {
            Serial.print("0");
        }

        Serial.print(rfid.uid.uidByte[i], HEX);
        Serial.print(" ");
    }

    Serial.println();

    // Detener comunicación con tarjeta
    rfid.PICC_HaltA();

    delay(1000);
}

/*ESP32 -- RFID
    SDA --	21
    SCK --	18
    MOSI--	19
    MISO--	23
    RST --	22
    GND --	GND
    3.3V--	3.3V*/