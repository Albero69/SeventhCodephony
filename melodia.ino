// TODO
// Aggiungere le pause alle note ripetute

#include <avr/pgmspace.h>

// Definizione della struttura Nota
struct Nota {
  int8_t pin;           // Numero del pin (int8_t per permettere -1)
  uint16_t durata;      // Durata in millisecondi
};

// Definizione delle costanti per le durate delle note
const uint16_t tono = 125;
const uint16_t croma = tono * 2;
const uint16_t croma_puntata = tono * 3;
const uint16_t semiminima = tono * 4;
const uint16_t semiminima_puntata = tono * 6;
const uint16_t minima = tono * 8;
const uint16_t minima_puntata = tono * 12;
const uint16_t semibreve = tono * 16;

// Definizione dei pin per le note
const uint8_t pin_DO = 2;
const uint8_t pin_RE = 3;
const uint8_t pin_MI = 4;
const uint8_t pin_FA = 5;
const uint8_t pin_SI = 6;
const uint8_t pin_LA = 7;
const uint8_t pin_REd = 8;
const uint8_t pin_DOd = 9;

const int8_t pausa = -1;

const char* const printNote[] PROGMEM = {
  "pin 1 null",
  "pin 1 null",
  "DO",
  "RE",
  "MI",
  "FA",
  "SI",
  "LA",
  "REd",
  "DOd"
};

// Array delle note della melodia, memorizzato in Flash
const Nota melodia[] PROGMEM = {
  // RIGA 1
  {pin_DO, semibreve },

  {pin_SI, semibreve },

  {pin_SI, tono },
  {pin_DO, tono },
  {pin_RE, semiminima },
  {pin_DO, tono },
  {pin_SI, tono },

  {pin_RE, croma },
  {pin_MI, croma },
  {pin_MI, semiminima },

  {pin_MI, semiminima_puntata },
  {pin_MI, tono },
  {pin_RE, tono },

  {pin_RE, croma },
  {pin_MI, croma },
  {pin_MI, croma },
  {pin_MI, croma },

  {pin_RE, tono },
  {pin_MI, tono },
  {pin_FA, semiminima_puntata },
  {pin_MI, tono },
  {pin_RE, tono },

  {pin_RE, croma },
  {pin_MI, croma },
  {pin_MI, semiminima },
  
  // RIGA 2
  {pin_MI, semibreve },

  {pin_REd, semibreve },

  {pin_RE, croma },
  {pin_SI, tono },
  {pin_DO, tono },
  {pin_RE, croma },
  {pin_MI, tono },
  {pin_RE, tono },

  {pin_REd, croma },
  {pin_DO, croma },
  {pin_DO, semiminima },

  {pin_RE, semibreve },

  {pin_SI, semiminima },
  {pin_DO, semiminima },

  {pin_SI, tono },
  {pin_DO, tono },
  {pin_RE, semiminima },
  {pin_DO, tono },
  {pin_SI, tono },

  {pin_LA, croma },
  {pin_LA, tono },
  {pin_SI, tono },
  {pin_DO, croma },
  {pin_DO, tono },
  {pin_RE, tono },

  {pin_MI, semibreve },
  
  // RIGA 3
  {pin_REd, semibreve },

  {pin_RE, croma },
  {pin_SI, tono },
  {pin_DOd, tono },
  {pin_RE, croma },
  {pin_MI, tono },
  {pin_RE, tono },

  {pin_REd, croma },
  {pin_DO, croma },
  {pin_DO, semiminima },

  {pin_DO, semibreve },

  {pin_SI, semiminima },
  {pin_DO, semiminima },

  {pin_SI, tono },
  {pin_DO, tono },
  {pin_RE, semiminima },
  {pin_DO, tono },
  {pin_SI, tono },

  {pin_LA, semiminima }
};

// Funzione per ottenere la lunghezza dell'array melodia
const size_t melodia_length = sizeof(melodia) / sizeof(melodia[0]);

void setup() {
  Serial.begin(9600); // Inizializza la comunicazione seriale

  // Imposta i pin come output, evitando di impostare la "pausa" come pin
  for (size_t i = 0; i < melodia_length; i++) {
    int8_t pin;
    memcpy_P(&pin, &melodia[i].pin, sizeof(pin)); // Per prelevare dati da flash
    if (pin != pausa) {
      pinMode(pin, OUTPUT);
    }
  }
}

void loop() {
  static size_t i = 0;
  static unsigned long previousMillis = 0;
  static bool isPlaying = false;
  static int8_t currentPin = pausa;  // Inizializza come pausa
  static unsigned long noteDuration = 0;

  if (i < melodia_length) {
    unsigned long currentMillis = millis();
    
    if (!isPlaying) { // Inizia una nuova nota
      Nota nota;
      memcpy_P(&nota, &melodia[i], sizeof(Nota));
      noteDuration = nota.durata;
      previousMillis = currentMillis;

      // Spegne l'ultima nota prima di suonare una nuova
      if (currentPin != pausa) {
        digitalWrite(currentPin, LOW);
      }

      currentPin = nota.pin;
      isPlaying = true;

      if (currentPin == pausa) {
        Serial.print("Pause for duration: ");
        Serial.println(noteDuration);
      } else {
        Serial.print("Playing note on pin: ");
        Serial.print(printNote[currentPin]);
        Serial.print(" with duration: ");
        Serial.println(noteDuration);
        digitalWrite(currentPin, HIGH);
      }
    } else if (currentMillis - previousMillis >= noteDuration) { // Tempo scaduto
      // Spegne la nota corrente alla fine della durata
      if (currentPin != pausa) {
        digitalWrite(currentPin, LOW);
      }
      isPlaying = false;
      i++; // Passa alla prossima nota
    }
  }
}