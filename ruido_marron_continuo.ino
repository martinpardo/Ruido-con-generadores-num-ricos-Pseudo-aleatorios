/* Ruido Marrón continuo y filtrado. 
*/

#include <Mozzi.h>
#include <mozzi_rand.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
#define FILTER_SHIFT 1 // 5 or 6 work well - the spectrum of 6 looks a bit more linear, like the generated brown noise in Audacity

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, MOZZI_AUDIO_RATE> aSin(SIN2048_DATA);

void setup()
{
  startMozzi(); // :)
  // No es necesario configurar la frecuencia del oscilador porque no lo usaremos
}

void updateControl()
{
  // No hay controles adicionales en este ejemplo
}

AudioOutput updateAudio()
{
  static int filtered;
  char whitenoise = rand((byte)255) - 128; // Genera ruido blanco (-128 a 127)
  filtered = filtered - (filtered >> FILTER_SHIFT) + whitenoise; // Filtra el ruido blanco
  int asig = filtered >> 3; // Ajusta el rango a 8 bits (valor entre -128 y 127)

  // Envía el ruido filtrado directamente sin modulación de amplitud
  return MonoOutput::from8Bit(asig);
}

void loop(){
  audioHook(); // Necesario para procesar el audio
}