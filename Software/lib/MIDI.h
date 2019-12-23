#include <stdint.h>

#define MIDI_NOTE_OFF 0x80 // really 0x8n same w/ on
#define MIDI_NOTE_ON 0x90

struct MIDI_note {
  char noteChars[2];
  char octaveChar;
};

int MIDI_isNoteOn(uint8_t message);
int MIDI_isNoteOff(uint8_t message);
struct MIDI_note MIDI_note(uint8_t message);
