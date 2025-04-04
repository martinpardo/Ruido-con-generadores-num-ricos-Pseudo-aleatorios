#include <MozziGuts.h>
#include <mozzi_rand.h>
#include <StateVariable.h>  // Filtro pasabanda de Mozzi

const int CONTROL_RATE = 64; // Tasa de actualización
StateVariable<int> svFilter; // CORREGIDO: Declarar correctamente la instancia del filtro

int filterFreq = 500;  // Frecuencia inicial del filtro
int freqStep = 50;  // Paso de variación de la frecuencia

void setup() {
  startMozzi(CONTROL_RATE); // Inicia Mozzi
  randomSeed(analogRead(0)); // Inicializa el generador pseudoaleatorio
}

void updateControl() {
  // Oscilar la frecuencia del filtro automáticamente
  filterFreq += freqStep;
  if (filterFreq >= 5000 || filterFreq <= 100) {
    freqStep = -freqStep;  // Invierte la dirección cuando llega a los límites
  }

  // Configurar el filtro pasabanda con frecuencia variable
  svFilter.setResonance(100);  // Resonancia media (0-255)
  svFilter.setCentreFreq(filterFreq, AUDIO_RATE);
}

int updateAudio() {
  // Generar ruido blanco en el rango [-32768, 32767]
  int white_noise = 2 * (rand() % 32768) - 32768;  // CORREGIDO: Usar rand() en lugar de mozziRandom()

  // Aplicar el filtro pasabanda
  return svFilter.bandPass(white_noise);
}

void loop() {
  audioHook(); // Requerido para que Mozzi funcione
}
