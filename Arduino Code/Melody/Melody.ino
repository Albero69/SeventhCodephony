/*
 * Melody.ino
 * 
 * Plays Beethoven’s Seventh Symphony using digital output pins.
 * Notes are defined in PROGMEM to conserve RAM.
 */

#include <avr/pgmspace.h> // For storing data in program memory (PROGMEM)

// Definition of the Note structure
struct Nota {
  int8_t pin;           // Pin number (int8_t allows using -1 to indicate a rest)
  uint16_t durata;      // Duration in milliseconds
};

// Duration constants [ms]
const uint16_t beat = 250;               // 250 ms
const uint16_t eighth_note = beat * 2;   // 500 ms
const uint16_t dotted_eighth = beat * 3; // 750 ms
const uint16_t quarter_note = beat * 4;  // 1000 ms
const uint16_t quintuplet = beat * 5;    // 1250 ms
const uint16_t dotted_quarter = beat * 7;// 1750 ms
const uint16_t half_note = beat * 8;     // 2000 ms

// Pin assignments for the musical notes
const uint8_t pin_RE   = 2; // D
const uint8_t pin_REd  = 3; // D#
const uint8_t pin_DOd  = 4; // C#
const uint8_t pin_DO   = 5; // C
const uint8_t pin_MI   = 6; // E
const uint8_t pin_FA   = 7; // F
const uint8_t pin_SI   = 8; // B
const uint8_t pin_LA   = 9; // A

const int8_t REST = -1; // Special value indicating a rest (no note)

// Melody array stored in flash memory (PROGMEM)
const Nota melody[] PROGMEM = {
  // Initialization block: turn all outputs off
  {2, 1 },
  {3, 1 },
  {4, 1 },
  {5, 1 },
  {6, 1 },
  {7, 1 },
  {8, 1 },
  {9, 1 },

  // ROW 1
  // Bar 23
  {pin_DO, half_note },
  // Bar 24
  {pin_SI, dotted_quarter },
  {REST, beat },
  // Bar 25
  {pin_SI, beat },
  {pin_DO, beat },
  {pin_RE, quarter_note },
  {pin_DO, beat },
  {pin_SI, beat },
  // Bar 26
  {pin_SI, eighth_note },
  {pin_DO, beat },
  {REST, beat },
  {pin_DO, dotted_eighth },
  {REST, beat },
  // Bar 27
  {pin_DO, quintuplet },
  {REST, beat },   
  {pin_DO, beat },
  {pin_RE, beat },
  // Bar 28
  {pin_REd, eighth_note },
  {pin_MI, beat },
  {REST, beat },
  {pin_MI, beat },
  {REST, beat },
  {pin_MI, eighth_note },
  // Bar 29
  {pin_RE, beat },
  {pin_MI, beat },
  {pin_FA, quarter_note },
  {pin_MI, beat },
  {pin_RE, beat },
  // Bar 30
  {pin_RE, eighth_note },
  {pin_MI, beat },
  {REST, beat },
  {pin_MI, dotted_eighth },
  {REST, beat },

  // ROW 2
  // Bar 31
  {pin_MI, half_note },
  // Bar 32
  {pin_REd, half_note },
  // Bar 33
  {pin_RE, eighth_note },
  {pin_SI, beat },
  {pin_DOd, beat },
  {pin_RE, eighth_note },
  {pin_MI, beat },
  {pin_RE, beat },
  // Bar 34
  {pin_RE, eighth_note },
  {pin_DOd, eighth_note },
  {pin_DO, dotted_eighth },
  {REST, beat },
  // Bar 35
  {pin_DO, half_note },
  // Bar 36
  {pin_SI, quarter_note },
  {pin_DO, quarter_note },
  // Bar 37
  {pin_SI, beat },
  {pin_DO, beat },
  {pin_RE, quarter_note },
  {pin_DO, beat },
  {pin_SI, beat },
  // Bar 38
  {pin_LA, beat },
  {REST, beat },
  {pin_LA, beat },
  {pin_SI, beat },
  {pin_DO, beat },
  {REST, beat },
  {pin_DO, beat },
  {pin_RE, beat },
  // Bar 39
  {pin_MI, half_note },

  // ROW 3
  // Bar 40
  {pin_REd, half_note },
  // Bar 41
  {pin_RE, eighth_note },
  {pin_SI, beat },
  {pin_DOd, beat },
  {pin_RE, eighth_note },
  {pin_MI, beat },
  {pin_RE, beat },
  // Bar 42
  {pin_RE, eighth_note },
  {pin_DOd, eighth_note },
  {pin_DO, dotted_eighth },
  {REST, beat },
  // Bar 43
  {pin_DO, half_note },
  // Bar 44
  {pin_SI, quarter_note },
  {pin_DO, quarter_note },
  // Bar 45
  {pin_SI, beat },
  {pin_DO, beat },
  {pin_RE, quarter_note },
  {pin_DO, beat },
  {pin_SI, beat },
  // Bar 46
  {pin_LA, quarter_note },
  {REST, quarter_note }
};

const size_t melodyLength = sizeof(melody) / sizeof(melody[0]);

void setup() {
  Serial.begin(9600);

  // Configure all the pins used in the melody as OUTPUT
  for (size_t i = 0; i < melodyLength; i++) {
    Nota currentNote;
    memcpy_P(&currentNote, &melody[i], sizeof(Nota));  // Read from flash memory

    if (currentNote.pin != REST) {
      pinMode(currentNote.pin, OUTPUT);  // Set pin as output
    }
  }
}

void loop() {
  static size_t position = 0;             // Current index in the melody
  static unsigned long lastTime = 0;      // Timestamp of when the current note started
  static bool noteActive = false;         // Whether a note is currently active
  static int8_t currentPin = REST;        // The pin currently set to HIGH
  static unsigned long noteDuration = 0;  // Duration of the current note

  if (position < melodyLength) {
    unsigned long now = millis();

    if (!noteActive) {  // Start the next note/rest
      Nota nextNote;
      memcpy_P(&nextNote, &melody[position], sizeof(Nota));  // Load next note from flash

      noteDuration = nextNote.durata;
      lastTime = now;

      // Turn off the previous note
      if (currentPin != REST) {
        digitalWrite(currentPin, LOW);
      }

      currentPin = nextNote.pin;
      noteActive = true;

      // Turn on the new note if it's not a rest
      if (currentPin != REST) {
        digitalWrite(currentPin, HIGH);
      }
    }
    else if (now - lastTime >= noteDuration) {  // Time to end the current note
      if (currentPin != REST) {
        digitalWrite(currentPin, LOW);
      }

      noteActive = false;
      position++;  // Move to the next note
    }
  }
  else {
    // End of melody reached, reset to beginning to loop continuously
    position = 0;
  }
}