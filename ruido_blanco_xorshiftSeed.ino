#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Mozzi.h>

// Crear una instancia del ADS1115
Adafruit_ADS1115 ads;

// Semilla para xorshiftSeed
static unsigned long xorshiftSeedValue = 123456789;

// Función para generar números aleatorios con xorshift
int xorshiftSeed() {
  xorshiftSeedValue ^= xorshiftSeedValue << 13;
  xorshiftSeedValue ^= xorshiftSeedValue >> 17;
  xorshiftSeedValue ^= xorshiftSeedValue << 5;
  return (int)(xorshiftSeedValue & 0xFF) - 128; // Rango: -128 a 127
}

void setup() {
  // Inicializar el ADS1115
  if (!ads.begin()) {
    Serial.println("Error al iniciar el ADS1115");
    while (1);
  }
  ads.setGain(GAIN_ONE); // Configurar ganancia del ADS1115 (ajusta según necesites)

  // Inicializar Mozzi
  startMozzi();
}

void updateControl() {
  // Leer el valor del canal A0 del ADS1115
  int adcValue = ads.readADC_SingleEnded(0); // Canal 0

  // Usar el valor del ADC para modular la semilla del ruido blanco
  xorshiftSeedValue = (unsigned long)adcValue + 123456789; // Modificar la semilla dinámicamente
}

AudioOutput updateAudio() {
  // Generar ruido blanco usando xorshiftSeed
  int whitenoise = xorshiftSeed();

  // Retornar el ruido blanco como salida de audio mono
  return MonoOutput::from8Bit(whitenoise);
}

void loop() {
  audioHook(); // Procesar audio en tiempo real
}