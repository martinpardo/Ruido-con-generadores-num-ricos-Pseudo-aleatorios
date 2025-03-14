// Variables globales para el filtro
float lowPassState = 0.0;   // Estado del filtro pasabajo
float highPassState = 0.0;  // Estado del filtro pasaaltos
float lowAlpha = 0.1;       // Factor de suavizado del pasabajo (ajustable)
float highAlpha = 0.1;      // Factor de suavizado del pasaaltos (ajustable)

// Función PRNG: Generador congruencial lineal
unsigned long state = 12345; // Semilla inicial
float generateNoise() {
    state = (1664525 * state + 1013904223) % 4294967296; // 2^32
    return (float)state / 4294967296.0; // Escala el valor a [0, 1]
}

// Filtro pasabajo
float lowPass(float input, float alpha, float &state) {
    state = alpha * input + (1.0f - alpha) * state;
    return state;
}

// Filtro pasaaltos
float highPass(float input, float alpha, float &state) {
    float previousState = state;
    state = alpha * (state + input - previousState);
    return state;
}

// Filtro pasabanda (combinación de pasaaltos y pasabajos)
float bandPass(float input, float lowAlpha, float highAlpha, float &lowState, float &highState) {
    float lowPassed = lowPass(input, lowAlpha, lowState);
    float bandPassed = highPass(lowPassed, highAlpha, highState);
    return bandPassed;
}

void setup() {
    pinMode(9, OUTPUT); // Configura el pin 9 como salida para PWM
}

void loop() {
    // Genera una muestra pseudoaleatoria
    float rawNoise = generateNoise();               // Ruido blanco sin filtrar

    // Aplica el filtro pasabanda
    float filteredNoise = bandPass(rawNoise, lowAlpha, highAlpha, lowPassState, highPassState);

    // Escala a rango 0-255 para PWM
    int audioSample = (filteredNoise + 1.0) * 127.5; // Convertir de [-1, 1] a [0, 255]

    // Salida de audio por PWM
    analogWrite(9, audioSample);

    // Control de la frecuencia de muestreo (~44.1 kHz)
    delayMicroseconds(23); // 1/44,100 segundos
}
 





