#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h> // Tabla de onda sinusoidal

// Parámetros del LCG
unsigned long seed = 12345; // Semilla inicial
const unsigned long a = 1664525;
const unsigned long c = 1013904223;
const unsigned long m = 4294967296; // 2^32

// Oscilador para generar audio
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> oscil(SIN2048_DATA);

void setup() {
  startMozzi(); // Inicializa Mozzi
}

unsigned long lcgRandom() {
  seed = (a * seed + c) % m; // Fórmula LCG
  return seed;
}

void updateControl() {
  // Generar un número pseudoaleatorio entre 0 y 255
  int randomValue = lcgRandom() >> 24; // Tomamos los primeros 8 bits

  // Usar el valor aleatorio para modular la frecuencia del oscilador
  int frequency = map(randomValue, 0, 255, 100, 1000); // Rango de frecuencia
  oscil.setFreq(frequency);
}

int updateAudio() {
  return oscil.next(); // Genera la señal de audio
}

void loop() {
  audioHook(); // Procesa el audio en tiempo real
}
