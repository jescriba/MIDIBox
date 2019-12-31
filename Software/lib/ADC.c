#include "ADC.h"
#include <avr/io.h>

void initADC() {
  ADMUX |= (1 << REFS0); // reference voltage on avcc
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1); // adc clock prescalr / 64
  ADCSRA |= (1 << ADEN); // enable adc
}

void startADC() {
  ADCSRA |= (1 << ADSC); // start the adc conversion
}
