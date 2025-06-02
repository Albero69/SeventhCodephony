/*
 * Accompaniment.ino
 * 
 * Plays Beethoven’s Seventh Symphony using digital output pins.
 * Notes are defined in PROGMEM to conserve RAM. The code alternates
 * between playing dual-note chords and single-note sequences with rests.
 */

#include <avr/pgmspace.h>  // For storing data in program memory (PROGMEM)

// Structure representing a musical note/chord
struct Note {
  int8_t pin1;      // First output pin (-1 for rest)
  int8_t pin2;      // Second output pin (-1 for rest)
  uint16_t duration; // Duration in milliseconds
};

// Note duration constants [ms]
const uint16_t EIGHTH = 125;             // Eighth note
const uint16_t DOTTED_QUARTER = 375;     // Dotted quarter note

// Pin definitions for musical notes
const uint8_t pin_DObasso = 2;   // Low C#
const uint8_t pin_SIbasso = 8;   // Low B
const uint8_t pin_MI = 3;        // E
const uint8_t pin_LAbasso = 13;  // Low A
const uint8_t pin_SI = 10;       // B
const uint8_t pin_LA = 12;       // A
const uint8_t pin_SOLdbasso = 6; // Low G#
const uint8_t pin_SOLd = 7;      // G#
const uint8_t pin_MIbasso = 11;  // Low E
const uint8_t pin_FAd = 4;       // F#
const uint8_t pin_SOLbasso = 9;  // Low G
const uint8_t pin_SOL = 5;       // G

const int8_t REST = -1;  // Rest indicator

// Melody sequence stored in PROGMEM (flash memory)
const Note melody[] PROGMEM = {
  // Initial test sequence (pins 2-13 activation test)
  {2, 3, 1}, {4, 5, 1}, {6, 7, 1}, {8, 9, 1}, {10, 11, 1}, {12, 13, 1},

  // Main musical sequence
  // Format: {Note1_Pin, Note2_Pin, Duration}
  // Uses defined pin constants and REST markers
  {pin_DObasso, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_DObasso, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_MI, pin_LA, EIGHTH},
  {REST, REST, EIGHTH},

  {pin_MI, pin_SOLd, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_MI, pin_SOLd, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
// -------------------------- //
  {pin_MI, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_MI, pin_MIbasso, EIGHTH},
  {REST, REST, EIGHTH},
  {pin_MI, pin_MIbasso, EIGHTH},
  {REST, REST, EIGHTH},

  {pin_MI, pin_LA, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_MI, pin_LA, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
// -------------------------- //
  {pin_MI, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_MI, pin_LA, EIGHTH},
  {REST, REST, EIGHTH},
  {pin_FAd, pin_LA, EIGHTH},
  {REST, REST, EIGHTH},

  {pin_SOL, pin_SOLbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_MI, pin_LA, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
// -------------------------- //
  {pin_SOL, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_SOL, pin_SOLbasso, EIGHTH},
  {REST, REST, EIGHTH},
  {pin_SOL, pin_SOLbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},

  {pin_SOL, pin_DObasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {REST, pin_DObasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
// -------------------------- //
  {pin_SOL, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_SOL, pin_DObasso, EIGHTH},
  {REST, REST, EIGHTH},
  {pin_LA, pin_DObasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},

  {pin_SI, pin_SIbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_SI, pin_SIbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
// -------------------------- //
  {pin_FAd, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_FAd, pin_MIbasso, EIGHTH},
  {REST, REST, EIGHTH},
  {pin_SOLd, pin_MIbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},

  {pin_LA, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_LA, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
// -------------------------- //
  {pin_MI, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_MI, pin_LAbasso, EIGHTH},
  {REST, REST, EIGHTH},
  {pin_MI, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},

  {pin_MI, pin_SOLbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_MI, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
// -------------------------- //
  {pin_MI, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_FAd, pin_MIbasso, EIGHTH},
  {REST, REST, EIGHTH},
  {pin_SOLd, pin_MIbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},

  {pin_LA, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {REST, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
// -------------------------- //
  {pin_SOL, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_SOL, pin_DObasso, EIGHTH},
  {REST, REST, EIGHTH},
  {pin_LA, pin_DObasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},

  {pin_SI, pin_SIbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_LA, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
// -------------------------- //
  {pin_FAd, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_FAd, pin_MIbasso, EIGHTH},
  {REST, REST, EIGHTH},
  {pin_SOLd, pin_MIbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},

  {pin_LA, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_LA, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
// -------------------------- //
  {pin_MI, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_MI, pin_LAbasso, EIGHTH},
  {REST, REST, EIGHTH},
  {pin_MI, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},

  {pin_MI, pin_SOLbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_MI, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
// -------------------------- //
  {pin_MI, REST, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {pin_FAd, pin_MIbasso, EIGHTH},
  {REST, REST, EIGHTH},
  {pin_SOLd, pin_MIbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},

  {pin_LA, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH},
  {REST, pin_LAbasso, DOTTED_QUARTER},
  {REST, REST, EIGHTH}
};

const size_t melodyLength = sizeof(melody) / sizeof(melody[0]);

void setup() {
  Serial.begin(9600);
  
  // Initialize all pins used in melody
  for (size_t i = 0; i < melodyLength; i++) {
    Note currentNote;
    memcpy_P(&currentNote, &melody[i], sizeof(Note));  // Load from PROGMEM
    
    if (currentNote.pin1 != REST) pinMode(currentNote.pin1, OUTPUT);
    if (currentNote.pin2 != REST) pinMode(currentNote.pin2, OUTPUT);
  }
}

void loop() {
  static size_t position = 0;           // Current position in melody
  static unsigned long lastTime = 0;     // Last note change time
  static bool notePlaying = false;       // Current note state
  static int8_t activePin1 = REST;       // Currently active pin 1
  static int8_t activePin2 = REST;       // Currently active pin 2
  static unsigned long currentDuration = 0; // Duration of current note

  if (position < melodyLength) {
    unsigned long now = millis();
    
    if (!notePlaying) {  // Start new note/chord
      Note nextNote;
      memcpy_P(&nextNote, &melody[position], sizeof(Note));  // Load from PROGMEM
      
      currentDuration = nextNote.duration;
      lastTime = now;

      // Turn off previous notes
      if (activePin1 != REST) digitalWrite(activePin1, LOW);
      if (activePin2 != REST) digitalWrite(activePin2, LOW);

      // Update current pins
      activePin1 = nextNote.pin1;
      activePin2 = nextNote.pin2;
      notePlaying = true;

      // Activate new pins
      if (activePin1 != REST) digitalWrite(activePin1, HIGH);
      if (activePin2 != REST) digitalWrite(activePin2, HIGH);
    }
    else if (now - lastTime >= currentDuration) {  // Note completed
      // Deactivate pins
      if (activePin1 != REST) digitalWrite(activePin1, LOW);
      if (activePin2 != REST) digitalWrite(activePin2, LOW);
      
      notePlaying = false;
      position++;  // Move to next note
    }
  }
  else {  // End of melody reached
    position = 0;  // Reset to start (loop forever)
  }
}