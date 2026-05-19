// Pines ESP32
const int EN1 = 25;
const int IN1 = 26;
const int IN2 = 27;

void setup() {
  pinMode(EN1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(EN1, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void loop() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(500);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(500);
}

/*Conexion de pines: (Nota, los gnd del L293D son los pines 4, 5, 12 y 13)
  esp32 -- L293D | BombaAgua -- L293D | L293D -- Motor? (Usa el USB del profe)
  3.3V  -> pin16 | Voltaje   -> pin3  | pin8  <- Voltaje
  GND   -> GND   | GND       -> pin6  | GND   <- GND  
  pin25 -> pin1  |
  pin26 -> pin2  | 
  pin27 -> pin7  | Nota, el input2 (pin7) no es necesario para controlar la bomba,
                   por lo que tambien puedes conectar el pin7 a GND.
*/