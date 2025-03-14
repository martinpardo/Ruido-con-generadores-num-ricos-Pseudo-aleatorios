#include <MozziGuts.h>
#include <Oscil.h>
#include <mozzi_rand.h> // Para xorshiftSeed()

// Parámetros del generador de ruido
const int NOISE_AMPLITUDE = 50; // Amplitud máxima del ruido (-127 a 127)

// Variables para xorshiftSeed()
unsigned long xorshift_seed = 123456789; // Semilla inicial

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
    // No se necesita control adicional en este ejemplo
}

int updateAudio() {
    // Genera un número pseudoaleatorio entre -127 y 127
    unsigned long rand_num = xorshiftSeed();
    int noise_sample = (rand_num % (NOISE_AMPLITUDE * 2)) - NOISE_AMPLITUDE;

    return noise_sample; // Devuelve la muestra de audio
}

void loop() {
    audioHook(); // Procesa el audio
}