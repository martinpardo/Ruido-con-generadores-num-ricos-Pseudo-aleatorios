/* Ruido blanco generado con xorshift_seed 
y filtro Lowpass
*/


#include <MozziGuts.h>
#include <mozzi_rand.h> // Para xorshiftSeed()

// Parámetros del generador de ruido
const int NOISE_AMPLITUDE = 50; // Amplitud máxima del ruido (-127 a 127)

// Variables para xorshiftSeed()
unsigned long xorshift_seed = 123456789; // Semilla inicial

// Filtro de paso bajo (implementación manual)
float lowPassFilter(float input, float cutoff_freq) {
    static float prev_output = 0.0; // Estado anterior del filtro
    float alpha = cutoff_freq / (cutoff_freq + (1.0 / AUDIO_RATE)); // Coeficiente de suavizado
    float output = alpha * input + (1.0 - alpha) * prev_output; // Fórmula del filtro
    prev_output = output; // Actualiza el estado anterior
    return output;
}

// Parámetros del filtro fluctuante
float cutoff_min = 100.0;  // Frecuencia de corte mínima (Hz)
float cutoff_max = 5000.0; // Frecuencia de corte máxima (Hz)
float cutoff_current = 1000.0; // Frecuencia de corte actual
int cutoff_change_speed = 1000; // Velocidad de cambio de la frecuencia de corte (ms)

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
}

void updateControl() {
    // Actualiza la frecuencia de corte del filtro de manera fluctuante
    unsigned long current_time = millis(); // Usa millis() para medir el tiempo
    if (current_time - last_cutoff_update > cutoff_change_speed) {
        // Genera una nueva frecuencia de corte aleatoria dentro del rango
        cutoff_current = map(xorshiftSeed() % 1000, 0, 1000, cutoff_min, cutoff_max);
        last_cutoff_update = current_time; // Guarda el tiempo de la actualización
    }
}

int updateAudio() {
    // Genera un número pseudoaleatorio entre -127 y 127
    unsigned long rand_num = xorshiftSeed();
    int noise_sample = (rand_num % (NOISE_AMPLITUDE * 2)) - NOISE_AMPLITUDE;

    // Aplica el filtro de paso bajo al ruido generado
    float filtered_sample_float = lowPassFilter(noise_sample, cutoff_current);

    // Convierte el resultado a un valor entero
    int filtered_sample = (int)filtered_sample_float;

    return filtered_sample; // Devuelve la muestra de audio filtrada
}

void loop() {
    audioHook(); // Procesa el audio
}


