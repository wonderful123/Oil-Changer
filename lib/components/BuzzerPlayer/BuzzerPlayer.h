#pragma once

#include "Logger.h"
#include "Melody.h"
#include "Notes.h"
#include <IBuzzer.h>

class BuzzerPlayer {
private:
  IBuzzer &_buzzer; // Reference to a buzzer object

public:
  explicit BuzzerPlayer(IBuzzer &buzzer) : _buzzer(buzzer) {}

  void playTune(MelodyName melodyName) {
    Melody melody = getMelody(melodyName);
    Logger::info("Starting to play tune");

    for (int thisNote = 0; thisNote < melody.length; thisNote++) {
      int noteDuration = melody.notes[thisNote].duration;
      Articulation articulation = melody.notes[thisNote].articulation;
      Dynamics dynamics = melody.notes[thisNote].dynamics;

      // Set the volume based on the dynamics
      switch (dynamics) {
      case DYNAMICS_SOFT:
        _buzzer.setVolume(0.25);
        break;
      case DYNAMICS_MEDIUM:
        _buzzer.setVolume(0.50);
        break;
      case DYNAMICS_LOUD:
        _buzzer.setVolume(1.00);
        break;
      }

      // Play the note
      _buzzer.beep(melody.notes[thisNote].frequency, noteDuration);

      // Determine the pause duration based on articulation
      int pauseBetweenNotes = noteDuration;
      if (articulation == ARTICULATION_STACCATO) {
        pauseBetweenNotes = noteDuration * 1.5; // Longer pause for staccato
      } else if (articulation == ARTICULATION_LEGATO) {
        pauseBetweenNotes = noteDuration * 0.8; // Shorter pause for legato
      }

      // Pause between notes
      _buzzer.beep(0, pauseBetweenNotes);
    }

    Logger::info("Finished playing tune");
  }
};