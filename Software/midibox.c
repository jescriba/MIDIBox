#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"
#include "AS1108.h"
#include "MIDI.h"

#include <stdio.h>
#include <stdlib.h>

#define NOTE_DURATION     0xF000        /* determines long note length */


// Refactor: Button interrupt
ISR(INT0_vect) {
  if (bit_is_clear(SWITCH_PIN, SWITCH)) {
    DISPLAY_setBrightness(0x0A);
  } else {
    DISPLAY_setBrightness(0x01);
  }
}

void initInterrupt0(void) {
  EIMSK |= (1 << INT0);
  EICRA |= (1 << ISC00);
  sei();
}

// Refactor: ADC setup

int main(void) {
  // HACK for flickering display on power up :/ investigate why
  _delay_ms(2000);
  initSPI();
  initUSART();
  initADC();
  initInterrupt0();

  SWITCH_PORT |= (1 << SWITCH); // input mode turn on pull up for switch

  uint8_t adcValue;

  DISPLAY_defaultConfig();
  DISPLAY_defaultTest();
  DISPLAY_writeChars("HOLA");

  // TODO PC0/ADC0 connected to the potentiometer

  while(1) {
    startADC();
    loop_until_bit_is_clear(ADCSRA, ADSC);
    adcValue = ADC; // read adc in
    DISPLAY_writeByte(DIGIT_0_ADDRESS, adcValue);

    // uint8_t message = receiveByte(); // assuming note off 8x or on 9x
    // DISPLAY_writeByte(DIGIT_3_ADDRESS, 0);
    // if (MIDI_isNoteOn(message) ) {
    //    uint8_t message2 = receiveByte(); // assuming note off 8x or on 9x
    //    uint8_t message3 = receiveByte(); // assuming note off 8x or on 9x
    //    struct MIDI_note note = MIDI_note(message2);
    //    DISPLAY_writeChar(DIGIT_0_ADDRESS, note.noteChars[0]);
    //    if (note.noteChars[1] == '-') {
    //      DISPLAY_writeChar(DIGIT_1_ADDRESS, note.octaveChar);
    //      DISPLAY_writeChar(DIGIT_2_ADDRESS, 0);
    //    } else {
    //      DISPLAY_writeChar(DIGIT_1_ADDRESS, note.noteChars[1]);
    //      DISPLAY_writeChar(DIGIT_2_ADDRESS, note.octaveChar);
    //    }
    //  } else if (MIDI_isNoteOff(message)) {
    //   uint8_t message2 = receiveByte(); // assuming note off 8x or on 9x
    //   uint8_t message3 = receiveByte(); // assuming note off 8x or on 9x
    //   DISPLAY_writeByte(DIGIT_0_ADDRESS, 0);
    // }
  }

  return 0;
}
