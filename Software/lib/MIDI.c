#include "MIDI.h"

int MIDI_isNoteOn(uint8_t message) {
  // bit math to check if message is 0x9n note on
  return (((message >> 4) & 0x0F) == (MIDI_NOTE_ON >> 4 & 0x0F));
}

int MIDI_isNoteOff(uint8_t message) {
  return (((message >> 4) & 0x0F) == (MIDI_NOTE_OFF >> 4 & 0x0F));
}

struct MIDI_note MIDI_note(uint8_t message) {
  // TODONOW
  struct MIDI_note note;
  note.octaveChar = ((message / 12) + '0'); // converts to char from 0-9
  switch (message % 12) {
    case 0:
      note.noteChars[0] = 'C';
      note.noteChars[1] = '-';
      return note;
    case 1:
      note.noteChars[0] = 'd';
      note.noteChars[1] = 'b'; // hack for flat
      return note;
    case 2:
      note.noteChars[0] = 'd';
      note.noteChars[1] = '-';
      return note;
    case 3:
      note.noteChars[0] = 'E';
      note.noteChars[1] = 'b';  // hack for flat
      return note;
    case 4:
    note.noteChars[0] = 'E';
    note.noteChars[1] = '-';
    return note;
    case 5:
    note.noteChars[0] = 'f';
    note.noteChars[1] = '-';
    return note;
    case 6:
    note.noteChars[0] = 'G';
    note.noteChars[1] = 'b';
    return note;
    case 7:
    note.noteChars[0] = 'G';
    note.noteChars[1] = '-';
    return note;
    case 8:
    note.noteChars[0] = 'A';
    note.noteChars[1] = 'b';
    return note;
    case 9:
    note.noteChars[0] = 'A';
    note.noteChars[1] = '-';
    return note;
    case 10:
    note.noteChars[0] = 'b';
    note.noteChars[1] = 'b';
    return note;
    case 11:
    note.noteChars[0] = 'b';
    note.noteChars[1] = '-'; // hack for sharp
    return note;
    default:
      note.noteChars[0] = '-';
      note.noteChars[1] = '-';
      return note;
  }
}
