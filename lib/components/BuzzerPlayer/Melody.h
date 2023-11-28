#pragma once

#include "Notes.h" // This includes the Note struct and note definitions

struct Melody {
  Note *notes;
  int length;
  const char *name; // Name of the melody
};

// Enumeration for different melodies
enum MelodyName {
  SUPER_MARIO_THEME,
  SUPER_MARIO_ARTICULATION_THEME,
  STARWARS_THEME,
  LAST_NINJA_INTRO_THEME,
  WONDERBOY_THEME
};

// Function to get a melody by name
Melody getMelody(MelodyName name);