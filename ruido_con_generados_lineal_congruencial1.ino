// C++ code
//

// Variables globales
unsigned long state = 12345; // Semilla inicial para el PRNG
float lowPassState = 0.0;    // Estado inicial del filtro pasabajo
float alpha = 0.1;           // Factor de suavizado del filtro (ajustable)

// Función PRNG: Generador congruencial lineal
float generateNoise() {
    state = (1664525 * state + 1013904223) % 4294967296; // 2^32
    return (float)state / 4294967296.0; // Escala el valor a [0, 1]
}

// Función filtro pasabajo
float lowPass(float input, float alpha, float &state) {
    state = alpha * input + (1.0f - alpha) * state;
    return state;
}

void setup() {
    pinMode(9, OUTPUT); // Configura el pin 9 como salida para PWM
}

void loop() {
    // Genera una muestra pseudoaleatoria y escala a rango 0-255
    float rawNoise = generateNoise()* 255;          // Ruido blanco sin filtrar
    float filteredNoise = lowPass(rawNoise, alpha, lowPassState); // Ruido filtrado
    int audioSample = filteredNoise * 255;    // Escala a 0-255 para PWM

    // Salida de audio por PWM
    analogWrite(9, audioSample);

    // Control de la frecuencia de muestreo (~44.1 kHz)
    delayMicroseconds(23); // 1/44,100 segundos
}

  
  
 
