#include <MozziGuts.h>
#include <Oscil.h>
#include <mozzi_rand.h> // Para xorshiftSeed()
#include <RollingAverage.h> // Para suavizar cambios
#include <FilterHighPass.h> // Filtro de paso alto

// Parámetros del generador de ruido
const int NOISE_AMPLITUDE = 127; // Amplitud máxima del ruido (-127 a 127)

// Variables para xorshiftSeed()
unsigned long xorshift_seed = 123456789; // Semilla inicial

// Filtro de paso alto
FilterHighPass highPassFilter;

// Parámetros del filtro fluctuante
int cutoff_min = 100;  // Frecuencia de corte mínima (Hz)
int cutoff_max = 5000; // Frecuencia de corte máxima (Hz)
int cutoff_current = 1000; // Frecuencia de corte actual
int cutoff_change_speed = 50; // Velocidad de cambio de la frecuencia de corte (ms)

unsigned long last_cutoff_update = 0; // Tiempo de la última actualización

// Función xorshiftSeed() para generar números pseudoaleatorios
unsigned long xorshiftSeed() {
    xorshift_seed ^= xorshift_seed << 13;
    xorshift_seed ^= xorshift_seed >> 17;
    xorshift_seed ^= xorshift_seed << 5;
    return xorshift_seed;
}

void setup() {
    startMozzi(); // Inicia Mozzi
    randomSeed(analogRead(0)); // Semilla aleatoria basada en ruido analógico
    xorshift_seed = random(1, 1000000); // Inicializa la semilla de xorshiftSeed

    // Configura el filtro de paso alto
    highPassFilter.setCutoff(cutoff_current);
}

void updateControl() {
    // Actualiza la frecuencia de corte del filtro de manera fluctuante
    unsigned long current_time = mozziMillis();
    if (current_time - last_cutoff_update > cutoff_change_speed) {
        // Genera una nueva frecuencia de corte aleatoria dentro del rango
        cutoff_current = map(xorshiftSeed() % 1000, 0, 1000, cutoff_min, cutoff_max);
        highPassFilter.setCutoff(cutoff_current); // Actualiza el filtro
        last_cutoff_update = current_time; // Guarda el tiempo de la actualización
    }
}

int updateAudio() {
    // Genera un número pseudoaleatorio entre -127 y 127
    unsigned long rand_num = xorshiftSeed();
    int noise_sample = (rand_num % (NOISE_AMPLITUDE * 2)) - NOISE_AMPLITUDE;

    // Aplica el filtro de paso alto al ruido generado
    int filtered_sample = highPassFilter.next(noise_sample);

    return filtered_sample; // Devuelve la muestra de audio filtrada
}

void loop() {
    audioHook(); // Procesa el audio
}