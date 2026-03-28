#include <U8g2lib.h>         // Librería para OLED SSD1306 128x64 — instalar desde Library Manager
                             // URL: https://github.com/olikraus/u8g2

#include <Button2.h>         // Manejo de botón con debounce — RESPALDO secundario
                             // URL: https://github.com/LennartHennworking/Button2


#include <Wire.h>            // I2C para comunicación con la OLED (incluida con ESP32 core)


// --- Pin del botón PRG ---
const int PIN_BOTON_PRG  = 0;   // Botón PRG integrado en la placa (activo LOW)

// --- Pin del LED integrado ---
const int PIN_LED        = 35;  // LED blanco integrado en la placa Heltec V3

// --- Pines de la pantalla OLED SSD1306 ---
const int PIN_OLED_SDA   = 17;  // Línea de datos I2C para OLED
const int PIN_OLED_SCL   = 18;  // Línea de reloj I2C para OLED
const int PIN_OLED_RST   = 21;  // Reset de la OLED (algunas placas usan -1 si no hay)

// --- Pin Vext: Alimentación de periféricos externos (OLED, sensores) ---
// En Heltec V3, el pin Vext (GPIO36) controla un regulador/switch que alimenta
// la pantalla OLED y otros periféricos. Se DEBE poner en LOW para encender la OLED.
// HIGH = periféricos apagados, LOW = periféricos encendidos
const int PIN_VEXT       = 36;

// --- Pantalla OLED 128x64 (U8g2) ---
// Constructor para SSD1306 128x64 por I2C con buffer de página completa (F = full buffer)
// U8G2_R0 = sin rotación; pines SDA/SCL se configuran después con Wire.begin()
U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, /* reset=*/ PIN_OLED_RST, /* clock=*/ PIN_OLED_SCL, /* data=*/ PIN_OLED_SDA);



void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  // --- Configurar el pin del LED ---
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  // --- Iniciar pantalla OLED ---
  // ¡¡CRÍTICO para Heltec V3!! Secuencia específica:
  //   1. Activar Vext  →  2. Reset OLED  →  3. Wire.begin  →  4. U8g2.begin

  // PASO 1: Activar Vext (GPIO36) — LOW = encender alimentación OLED
  pinMode(PIN_VEXT, OUTPUT);
  digitalWrite(PIN_VEXT, LOW);
  delay(100);

  // PASO 2: Reset hardware OLED (ciclo LOW → HIGH en pin RST)
  pinMode(PIN_OLED_RST, OUTPUT);
  digitalWrite(PIN_OLED_RST, LOW);
  delay(50);
  digitalWrite(PIN_OLED_RST, HIGH);
  delay(50);

  // PASO 3: Inicializar I2C con pines correctos
  Wire.begin(PIN_OLED_SDA, PIN_OLED_SCL);

  // PASO 4: Inicializar U8g2
  oled.begin();
  oled.setContrast(255);

  Serial.println("[OLED] Pantalla inicializada correctamente");
  // PASO 5: Test rápido
  testOLED();

  // Mostrar pantalla de bienvenida
  mostrarPantallaBienvenida();

  // Paso 1: Configurar GPIO0 como entrada con pull-up
  pinMode(PIN_BOTON_PRG, INPUT_PULLUP);
  
}

// the loop routine runs over and over again forever:
void loop() {

}


void testOLED() {
  oled.clearBuffer();
  oled.setFont(u8g2_font_ncenB08_tr);
  oled.drawStr(30, 15, "OLED OK!");
  oled.setFont(u8g2_font_6x10_tr);
  oled.drawStr(10, 35, "Pantalla funciona");
  oled.drawStr(10, 50, "Heltec V3 Ready");
  oled.sendBuffer();
  Serial.println("[OLED] Test OLED: Mensaje de prueba mostrado");
  delay(2000);
}

void mostrarPantallaBienvenida() {
  oled.clearBuffer();                                   // Limpiar buffer de pantalla

  oled.setFont(u8g2_font_ncenB10_tr);                  // Fuente mediana negrita
  oled.drawStr(10, 15, "Welcome");                   // Título línea 1

  oled.setFont(u8g2_font_ncenB08_tr);                  // Fuente más pequeña
  oled.drawStr(15, 30, "carjavi");                  // Mostrar ID del transmisor

  oled.drawStr(5, 45, "Heltec V3 + RadioLib");          // Info de hardware
  oled.drawStr(20, 60, "Iniciando...");                 // Estado

  oled.sendBuffer();                                    // Enviar buffer a la pantalla
  delay(2000);
}