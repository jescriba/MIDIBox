#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"
#include "AS1108.h"
#include "MIDI.h"

struct state {
  uint8_t index;
  uint8_t switchFlag;
  uint8_t switchValue;
  uint8_t knobFlag;
  uint8_t knobValue;
  uint8_t midiRXFlag;
  uint8_t midiTXFlag;
};

struct state currentState = { 0, 0, 0, 0, 0, 0, 0 };
struct state *currentStatePtr = &currentState;

// Refactor: Button interrupt
ISR(INT0_vect) {
  if (bit_is_clear(SWITCH_PIN, SWITCH)) {
    currentStatePtr->switchValue = 1;
  } else {
    currentStatePtr->switchValue = 0;
  }
  currentStatePtr->switchFlag = 1;
}

void initInterrupt0(void) {
  EIMSK |= (1 << INT0);
  EICRA |= (1 << ISC00);
  sei();
}

void didChangeSwitch() {
  if (currentStatePtr->switchValue) {
    DISPLAY_setBrightness(0x0A);
  } else {
    DISPLAY_setBrightness(0x01);
  }
}

// Refactor: ADC setup

ISR(ADC_vect) {
  currentStatePtr->knobFlag = 1;
  currentStatePtr->knobValue = ADCH;
  startADC();
}

void initADCInterrupt() {
  // Enable ADC interrupts
  ADCSRA |= (1 << ADIE);
}

void didChangeKnob() {
  int currentModeValue = currentStatePtr->knobValue % 9;
  if (currentStatePtr->index == currentModeValue) {
    return;
  }
  for (int i = 0; i < 9; i++) {
    if (currentModeValue == i) {
      char displayVal = currentModeValue + '0';
      char displayValues[4] = { '0', '0', '0', displayVal };
      DISPLAY_writeChars(displayValues);
    }
  }
  currentStatePtr->index = currentModeValue;
}

// Refactor: UART

ISR(USART_RX_vect) {
  currentStatePtr->midiRXFlag = 1;
  insertByte(UDR0);
}

uint8_t messages[3];
uint8_t messageCount = 0;

void insertByte(uint8_t byte) {
  if (messageCount >= 3) {
    messageCount = 0;
  }
  messages[messageCount] = byte;
  messageCount++;
}

void didReceiveMIDI() {
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

  if (messageCount != 3) {
    return;
  }
  if (MIDI_isNoteOn(messages[0])) {
    struct MIDI_note note = MIDI_note(messages[1]);
    DISPLAY_writeChar(DIGIT_0_ADDRESS, note.noteChars[0]);
     if (note.noteChars[1] == '-') {
       DISPLAY_writeChar(DIGIT_1_ADDRESS, note.octaveChar);
       DISPLAY_writeChar(DIGIT_2_ADDRESS, 0);
     } else {
       DISPLAY_writeChar(DIGIT_1_ADDRESS, note.noteChars[1]);
       DISPLAY_writeChar(DIGIT_2_ADDRESS, note.octaveChar);
     }
     DISPLAY_writeByte(DIGIT_3_ADDRESS, 0);
  }
}


ISR(USART_TX_vect) {
  currentStatePtr->midiTXFlag = 1;
}

void didTransmitMIDI() {

}

void initUARTInterrupt() {
  UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0);
}

int main(void) {
  // HACK for flickering display on power up :/ investigate why
  _delay_ms(2000);
  initSPI();
  initUSART();
  initADC();

  SWITCH_PORT |= (1 << SWITCH); // input mode turn on pull up for switch

  DISPLAY_defaultConfig();
  DISPLAY_defaultTest();
  DISPLAY_writeChars("HOLA");

  initInterrupt0();
  initADCInterrupt();
  initUARTInterrupt();

  // TODO PC0/ADC0 connected to the potentiometer
  startADC();
  while(1) {
    // TODO DRY
    if (currentStatePtr->switchFlag == 1) {
      cli();
      didChangeSwitch();
      currentStatePtr->switchFlag = 0;
      sei();
    }
    if (currentStatePtr->knobFlag == 1) {
      cli();
      didChangeKnob();
      currentStatePtr->knobFlag = 0;
      sei();
    }
    if (currentStatePtr->midiRXFlag == 1) {
      cli();
      currentStatePtr->midiRXFlag = 0;
      didReceiveMIDI();
      sei();
    }
    if (currentStatePtr->midiTXFlag == 1) {
      cli();
      currentStatePtr->midiTXFlag = 0;
      didTransmitMIDI();
      sei();
    }
  }

  return 0;
}
