// BuzzerPlayer.h
#pragma once

#ifdef PLATFORM_ESP32

#include "Logger.h"
#include "Melody.h"
#include "Notes.h"
#include <IBuzzer.h>
#include <Ticker.h>
#include <queue>

class BuzzerPlayer {
private:
  IBuzzer &_buzzer;
  Ticker _noteTimer;
  std::queue<MelodyID> _tuneQueue;
  Melody _currentMelody;
  float _notePauseFactor = 1.0;
  int _currentNoteIndex = 0;
  bool _isPlaying = false;

  void playNote() {
    if (_currentNoteIndex < _currentMelody.length) {
      Note note = _currentMelody.notes[_currentNoteIndex];

      // Set the volume based on the dynamics
      switch (note.dynamics) {
      case DYNAMICS_SOFT:
        _buzzer.adjustVolume(0.25);
        break;
      case DYNAMICS_MEDIUM:
        _buzzer.adjustVolume(0.50);
        break;
      case DYNAMICS_LOUD:
        _buzzer.adjustVolume(1.00);
        break;
      }

      // Play the note
      _buzzer.beep(note.frequency, note.duration);

      // Determine the pause duration based on articulation
      int pauseDuration = note.duration;
      if (note.articulation == ARTICULATION_STACCATO) {
        pauseDuration *= 1.5; // Longer pause for staccato
      } else if (note.articulation == ARTICULATION_LEGATO) {
        pauseDuration *= 0.8; // Shorter pause for legato
      }

      pauseDuration *= _notePauseFactor; // Include a slight pause between notes

      _currentNoteIndex++;
      // Schedule the next note or pause
      _noteTimer.once_ms(pauseDuration, noteTimerCallback, this);
    } else {
      finishTune();
    }
  }

  static void noteTimerCallback(BuzzerPlayer *player) {
    if (player) {
      player->playNote();
    }
  }

  void finishTune() {
    Logger::info("[BuzzerPlayer] Finished playing tune");
    _currentNoteIndex = 0;
    _isPlaying = false;
    playNextInQueue();
  }

  void playNextInQueue() {
    if (!_tuneQueue.empty()) {
      MelodyID nextTune = _tuneQueue.front();
      _tuneQueue.pop();
      playTune(nextTune);
    }
  }

public:
  explicit BuzzerPlayer(IBuzzer &buzzer) : _buzzer(buzzer) {}

  void playTune(MelodyID id) {
    if (!_isPlaying) {
      _isPlaying = true;
      _currentMelody = getMelody(id);
      _currentNoteIndex = 0;
      Logger::info("[BuzzerPlayer] Starting to play tune: " + _currentMelody.name);
      playNote(); // Start playing the first note
    } else {
      Logger::info("[BuzzerPlayer] Enqueueing tune: " + getMelodyName(id));
      _tuneQueue.push(id);
    }
  }
};

#endif // PLATFORM_ESP32