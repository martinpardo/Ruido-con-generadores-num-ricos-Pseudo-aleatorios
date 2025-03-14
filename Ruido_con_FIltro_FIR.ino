// Coeficientes FIR generados (ejemplo para un filtro pasabanda de 500-5000 Hz)
const int numTaps = 101; // Número de coeficientes
float coefficients[numTaps] = {
    -0.0002, -0.0003, 0.0005, 0.0012, 0.0018, 0.0007, -0.0026, -0.0066, 
    // (continúa con el resto de los coeficientes)
    0.0033, 0.0022, 0.0011, 0.0004, -0.0001, -0.0002
};

// Buffer para almacenar muestras
float buffer[numTaps] = {0};

// Función PRNG: Generador congruencial lineal
unsigned long state = 12345; // Semilla inicial
float generateNoise() {
    state = (1664525 * state + 1013904223) % 4294967296; // 2^32
    return (float)state / 4294967296.0; // Escala el valor a [0, 1]
}

// Aplicar el filtro FIR
float applyFIR(float input) {
    // Desplaza las muestras en el buffer
    for (int i = numTaps - 1; i > 0; i--) {
        buffer[i] = buffer[i - 1];
    }
    buffer[0] = input;

    // Calcula la salida del filtro
    float output = 0;
    for (int i = 0; i < numTaps; i++) {
        output += coefficients[i] * buffer[i];
    }
    return output;
}

void setup() {
    pinMode(9, OUTPUT); // Configura el pin 9 como salida para PWM
}

void loop() {
    // Genera una muestra pseudoaleatoria
    float rawNoise = generateNoise();               // Ruido blanco sin filtrar

    // Aplica el filtro FIR pasabanda
    float filteredNoise = applyFIR(rawNoise);

    // Escala a rango 0-255 para PWM
    int audioSample = (filteredNoise + 1.0) * 127.5; // Convertir de [-1, 1] a [0, 255]

    // Salida de audio por PWM
    analogWrite(9, audioSample);

    // Control de la frecuencia de muestreo (~44.1 kHz)
    delayMicroseconds(23); // 1/44,100 segundos
}
