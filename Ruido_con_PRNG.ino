// Variables globales
unsigned long state = 12345; // Semilla inicial para el PRNG

// Función PRNG: Generador congruencial lineal
float generateNoise() {
    state = (1664525 * state + 1013904223) % 4294967296; // 2^32
    return (float)state / 4294967296.0; // Escala el valor a [0, 1]
}

void setup() {
    pinMode(9, OUTPUT); // Configura el pin 9 como salida para PWM
}

void loop() {
    // Genera una muestra pseudoaleatoria y escala a rango 0-255
    int audioSample = generateNoise() * 255;

    // Salida de audio por PWM
    analogWrite(9, audioSample);

    // Control de la frecuencia de muestreo
    delayMicroseconds(23); // ~44.1 kHz (1 / 44,100 segundos)
}
