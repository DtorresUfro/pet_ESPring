#define BOMBA_EN 25
#define BOMBA_IN 26

void setup()
{
    Serial.begin(9600);

    delay(2000);

    Serial.println("Prueba L293D + bomba");

    pinMode(BOMBA_EN, OUTPUT);
    pinMode(BOMBA_IN, OUTPUT);

    // Apagar inicialmente
    digitalWrite(BOMBA_EN, LOW);
    digitalWrite(BOMBA_IN, LOW);
}

void loop()
{
    Serial.println("BOMBA ENCENDIDA");

    // Activar L293D
    digitalWrite(BOMBA_EN, HIGH);
    digitalWrite(BOMBA_IN, HIGH);

    delay(3000);

    Serial.println("BOMBA APAGADA");

    // Apagar
    digitalWrite(BOMBA_EN, LOW);
    digitalWrite(BOMBA_IN, LOW);

    delay(3000);
}
/*Conexion de pines: (Nota, los gnd del L293D son los pines 4, 5, 12 y 13)
  esp32 -- L293D | BombaAgua -- L293D | L293D -- Motor? (Usa el USB del profe)
  3.3V  -> pin16 | Voltaje   -> pin3  | pin8  <- Voltaje
  GND   -> GND   | GND       -> pin6  | GND   <- GND  
  pin25 -> pin1  |
  pin26 -> pin2  | 
  NOTA: El pin7 del L293D tiene que ir a tierra del mismo.*/