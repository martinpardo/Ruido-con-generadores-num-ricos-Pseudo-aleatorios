/*
XorshiftSeed con passBand
*/


#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Mozzi.h>


// creo una instancia para el ADS1115

Adafruit_ADS1115 ads;

// Semilla para XorshiftSeed 

static unsigned long xorshiftSeedValue = 123456789;

// PassBand Filter

float x1 = 0 ,x2 = 0; //entradas pasadas
float y1 = 0 ,y2 = 0; //salidas  pasadas

// Coeficientes del filtro PassBand ( ajustables)

const float a0 = 0.0147;
const float a1 = 0.0;
const float a2 = -0.0147;
const float b1 = -1.7576;
const float b2 = 0.7749;

//Funcion para generer numeros aleatorios con xorshift
int xorshiftSeed(){
  xorshiftSeedValue ^= xorshiftSeedValue << 13;
  xorshiftSeedValue ^= xorshiftSeedValue << 17;
  xorshiftSeedValue ^= xorshiftSeedValue << 5;
  return (int)(xorshiftSeedValue & 0xFF) -128; // rango -128 a 127
}


void setup() {
  if (!ads.begin()) {
    Serial.println("Error al iniciar el ADS1115");
    while(1);
  }
ads.setGain(GAIN_ONE); // Configura la ganancia del ADS1115
startMozzi();
}

void updateControl(){
  //leer el valor del canal A0 del ADS1115
  int adcValue = ads.readADC_SingleEnded(0); // canal 0

  //usar el valro del ADC para modular la semilla del ruido blanco
  xorshiftSeedValue = (unsigned long)adcValue + 123456789;  // modifica la semilla dinamicamente
}

AudioOutput updateAudio(){
  //generar ruido blanco usando xorshiftSeed 
  int whitenoise = xorshiftSeed();

  //aplicar el passBand filter

  float x0=(float)whitenoise / 128.0 ;// normaliza la entrada (-1a1 )
  float y0= a0*x0+a1*x1+a2*x2-b1*y1-b2*y2;

  //actualizar  las  variables de estado 

  x2 = x1;
  x1 = x0;
  y2 = y1;
  y1 = y0;

  // escalar la salida del filtro al rango del 8 bits (-128 a 127)

  int filteredSignal = (int) (y0 * 128.0);

  // retornar la señal filtrada como salida de audio mono

  return MonoOutput:: from8Bit(filteredSignal);
}

void loop() {
    audioHook(); // Procesar el audio en tiempo real

}
