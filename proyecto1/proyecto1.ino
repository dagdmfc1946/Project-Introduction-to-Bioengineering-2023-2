#include "BluetoothSerial.h"
#include <LiquidCrystal.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it.
#endif

// Se inicializan las variables a usar
const int portPin = 34;  // Pin ESP32
int potValor = 0; // Valor del potenciómetro
float potValorVoltaje = 0;
float GradosVoltaje = 0;

BluetoothSerial SerialBT;

// Definición del carácter personalizado para el símbolo de grados (°)
byte customDegreeChar[8] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};

LiquidCrystal lcd(22, 23, 5, 18, 19, 21);

void setup() {
  SerialBT.begin("BluetoothESP32");
  //Serial.begin(9600); // Puerto serial para mostrar por consola
  Serial.begin(115200); // Puerto serial para mostrar por consola

  // Inicializa la pantalla LCD
  lcd.createChar(0, customDegreeChar); // Crea el carácter personalizado (símbolo de grados '°')
  lcd.begin(16, 2); // Inicializa la pantalla LCD 
  lcd.clear();
  lcd.setCursor(0, 0); // Posición para mostrar el valor
  lcd.print("Angulo: "); // Imprime texto en la pantalla LCD

  lcd.setCursor(0, 1); // Posición para mostrar el valor
  lcd.print("Voltaje: "); // Imprime texto en la pantalla LCD
}

void loop() {
  potValor = analogRead(portPin); // Leer valor del potenciómetro
  potValorVoltaje = float(potValor);
  potValorVoltaje = potValorVoltaje * (3.3 / 4095);
  GradosVoltaje = potValorVoltaje * (260 / 3.3);
  Serial.println(float(GradosVoltaje)); // Imprime el valor generado por el potenciómetro

  // Muestra la medida en grados en la pantalla LCD
  lcd.setCursor(9, 0); // Posición para mostrar el valor
  lcd.print("     "); // Limpia la línea para evitar problemas de visualización
  lcd.setCursor(9, 0); // Posición para mostrar el valor
  lcd.print(float(GradosVoltaje), 2); // Muestra el valor con 2 decimales en la pantalla LCD
  lcd.write((uint8_t)0); // Escribe el carácter personalizado (símbolo de grados '°')

  lcd.setCursor(9, 1); // Posición para mostrar el valor
  lcd.print("     "); // Limpia la línea para evitar problemas de visualización
  lcd.setCursor(9, 1); // Posición para mostrar el valor
  lcd.print(float(potValorVoltaje), 2); // Muestra el valor con 2 decimales en la pantalla LCD
  lcd.setCursor(15, 1); // Posición para mostrar el valor
  lcd.print("V"); // Muestra el valor con 2 decimales en la pantalla LCD

  SerialBT.print(GradosVoltaje);
  SerialBT.print(";");
  delay(1000);
}
