/*
Ruido marron con ASD1115


*/
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Mozzi.h>
#include <mozzi_rand.h>

// Crear una instancia del ADS1115
Adafruit_ADS1115 ads;

// Parámetros del filtro
#define FILTER_SHIFT_MIN 4  // Valor mínimo para el grado de filtrado
#define FILTER_SHIFT_MAX 8  // Valor máximo para el grado de filtrado

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
  
  // Mapear el valor del ADC al rango de FILTER_SHIFT_MIN a FILTER_SHIFT_MAX
  int filterShift = map(adcValue, 0, 32767, FILTER_SHIFT_MIN, FILTER_SHIFT_MAX);
  
  // Actualizar el grado de filtrado
  setFilterShift(filterShift);
}

// Variable estática para almacenar el estado del filtro
static int filtered;
static int currentFilterShift = 6; // Valor inicial del filtro

// Función para actualizar el grado de filtrado
void setFilterShift(int filterShift) {
  currentFilterShift = filterShift;
}

AudioOutput updateAudio() {
  // Generar ruido blanco (-128 a 127)
  char whitenoise = rand((byte)255) - 128;

  // Filtrar el ruido blanco para convertirlo en ruido marrón
  filtered = filtered - (filtered >> currentFilterShift) + whitenoise;

  // Escalar el ruido marrón al rango de 8 bits (-128 a 127)
  int asig = filtered >> 3;

  // Retornar el ruido marrón como salida de audio mono
  return MonoOutput::from8Bit(asig);
}

void loop() {
  audioHook(); // Procesar audio en tiempo real
}