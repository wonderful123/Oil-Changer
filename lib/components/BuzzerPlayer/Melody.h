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
  STARWARS_THEME,
  // Add more melody names here
};

// Function to get a melody by name
Melody getMelody(MelodyName name);