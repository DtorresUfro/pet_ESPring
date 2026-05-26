#include <HX711.h>

// Pines HX711
#define HX_DT 4
#define HX_SCK 27

HX711 balanza;

// Ajustar luego con calibración real
float factorCalibracion = -7050.0;

void setup()
{
    Serial.begin(9600);

    delay(2000);

    Serial.println("Iniciando HX711...");

    // Inicializar HX711
    balanza.begin(HX_DT, HX_SCK);

    // Configurar escala
    balanza.set_scale(factorCalibracion);

    Serial.println("Realizando tara...");

    // Tara
    balanza.tare();

    Serial.println("HX711 listo");
}

void loop()
{
    // Promedio de 5 lecturas
    float lectura = balanza.get_units(5);

    // Convertir a entero
    int peso = (int)lectura;

    // Evitar negativos pequeños
    if (peso < 0)
    {
        peso = 0;
    }

    Serial.print("Peso: ");
    Serial.println(peso);

    delay(500);
}