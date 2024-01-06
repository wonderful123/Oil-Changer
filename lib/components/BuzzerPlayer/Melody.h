// Melody.h
#pragma once

#include <string>

#include "Notes.h"

struct Melody {
  Note *notes;
  int length;
  std::string name;  // Name of the melody
};

// Enumeration for different melodies
enum MelodyID {
  SUPER_MARIO_THEME,
  SUPER_MARIO_ARTICULATION_THEME,
  STARWARS_THEME,
  LAST_NINJA_INTRO_THEME,
  WONDERBOY_THEME
};

// Function to get a melody by name
Melody getMelody(MelodyID id);

std::string getMelodyName(MelodyID id);