#include "Melody.h"

// Define the notes for the Super Mario melody
Note superMarioNotes[] = {
    {NOTE_E4, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, {NOTE_E4, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, 
    {NOTE_REST, 125, ARTICULATION_NORMAL, DYNAMICS_SOFT}, {NOTE_E4, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, 
    {NOTE_REST, 125, ARTICULATION_NORMAL, DYNAMICS_SOFT}, {NOTE_C4, 125, ARTICULATION_LEGATO, DYNAMICS_MEDIUM}, 
    {NOTE_E4, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, {NOTE_REST, 125, ARTICULATION_NORMAL, DYNAMICS_SOFT}, 
    {NOTE_G4, 125, ARTICULATION_STACCATO, DYNAMICS_LOUD}, {NOTE_REST, 375, ARTICULATION_NORMAL, DYNAMICS_SOFT}, 
    {NOTE_G3, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, {NOTE_REST, 375, ARTICULATION_NORMAL, DYNAMICS_SOFT},
    {NOTE_C4, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, {NOTE_REST, 250, ARTICULATION_NORMAL, DYNAMICS_SOFT}, 
    {NOTE_G3, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, {NOTE_REST, 250, ARTICULATION_NORMAL, DYNAMICS_SOFT}, 
    {NOTE_E3, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, {NOTE_REST, 250, ARTICULATION_NORMAL, DYNAMICS_SOFT}, 
    {NOTE_A3, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, {NOTE_B3, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, 
    {NOTE_AS3, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, {NOTE_A3, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, 
    {NOTE_REST, 125, ARTICULATION_NORMAL, DYNAMICS_SOFT}, {NOTE_G3, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, 
    {NOTE_E4, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, {NOTE_G4, 125, ARTICULATION_STACCATO, DYNAMICS_LOUD}, 
    {NOTE_A4, 125, ARTICULATION_STACCATO, DYNAMICS_LOUD}, {NOTE_REST, 125, ARTICULATION_NORMAL, DYNAMICS_SOFT}, 
    {NOTE_F4, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, {NOTE_G4, 125, ARTICULATION_STACCATO, DYNAMICS_LOUD}, 
    {NOTE_REST, 125, ARTICULATION_NORMAL, DYNAMICS_SOFT}, {NOTE_E4, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, 
    {NOTE_REST, 125, ARTICULATION_NORMAL, DYNAMICS_SOFT}, {NOTE_C4, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, 
    {NOTE_D4, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, {NOTE_B3, 125, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}, 
    {NOTE_REST, 125, ARTICULATION_NORMAL, DYNAMICS_SOFT}, {NOTE_REST, 125, ARTICULATION_NORMAL, DYNAMICS_SOFT}
};


Note marioThemeNotesWithArticulation[] = {
    // Intro
    {NOTE_E4, 250, ARTICULATION_STACCATO},
    {NOTE_E4, 250, ARTICULATION_STACCATO},
    {NOTE_E4, 250, ARTICULATION_STACCATO},
    {NOTE_C5, 500, ARTICULATION_LONG},
    {NOTE_E4, 250},
    {NOTE_G4, 250, ARTICULATION_STACCATO},
    {NOTE_E4, 250, ARTICULATION_STACCATO},
    {NOTE_C4, 500, ARTICULATION_LONG},
    {NOTE_E4, 250},
    {NOTE_G4, 250, ARTICULATION_STACCATO},

    // Main melody
    {NOTE_C5, 250, ARTICULATION_LEGATO},
    {NOTE_D5, 250, ARTICULATION_LEGATO},
    {NOTE_E5, 250, ARTICULATION_LEGATO},
    {NOTE_F5, 250, ARTICULATION_LEGATO},
    {NOTE_G5, 500, ARTICULATION_LEGATO},
    {NOTE_C5, 250, ARTICULATION_LEGATO},
    {NOTE_D5, 250, ARTICULATION_LEGATO},
    {NOTE_E5, 250, ARTICULATION_LEGATO},
    {NOTE_F5, 250, ARTICULATION_LEGATO},
    {NOTE_G5, 500, ARTICULATION_LEGATO},

    // Variation
    {NOTE_E5, 250, ARTICULATION_STACCATO},
    {NOTE_F5, 250, ARTICULATION_STACCATO},
    {NOTE_G5, 250, ARTICULATION_STACCATO},
    {NOTE_A5, 250, ARTICULATION_STACCATO},
    {NOTE_B5, 500, ARTICULATION_LONG},
    {NOTE_E5, 250, ARTICULATION_STACCATO}};

Note starWarsIntroNotes[] = {
    {NOTE_A3, 500},  {NOTE_A3, 500},   {NOTE_A3, 500},  {NOTE_F3, 375},
    {NOTE_C4, 125},  {NOTE_A3, 500},   {NOTE_F3, 375},  {NOTE_C4, 125},
    {NOTE_A3, 1000}, {NOTE_E4, 500},   {NOTE_E4, 500},  {NOTE_E4, 500},
    {NOTE_F4, 375},  {NOTE_C4, 125},   {NOTE_GS3, 500}, {NOTE_F3, 375},
    {NOTE_C4, 125},  {NOTE_A3, 1000},  {NOTE_A4, 500},  {NOTE_A3, 375},
    {NOTE_A3, 125},  {NOTE_A4, 500},   {NOTE_GS4, 375}, {NOTE_G4, 125},
    {NOTE_FS4, 125}, {NOTE_E4, 125},   {NOTE_F4, 250},  {NOTE_REST, 250},
    {NOTE_AS3, 250}, {NOTE_DS4, 500},  {NOTE_D4, 375},  {NOTE_CS4, 125},
    {NOTE_C4, 125},  {NOTE_B3, 125},   {NOTE_C4, 250},  {NOTE_REST, 250},
    {NOTE_F3, 250},  {NOTE_GS3, 500},  {NOTE_F3, 375},  {NOTE_C4, 125},
    {NOTE_A3, 500},  {NOTE_A3, 375},   {NOTE_A3, 125},  {NOTE_A4, 500},
    {NOTE_GS4, 375}, {NOTE_G4, 125},   {NOTE_FS4, 125}, {NOTE_E4, 125},
    {NOTE_F4, 250},  {NOTE_REST, 250}, {NOTE_AS3, 250}, {NOTE_DS4, 500},
    {NOTE_D4, 375},  {NOTE_CS4, 125},  {NOTE_C4, 125},  {NOTE_B3, 125},
    {NOTE_C4, 250},  {NOTE_REST, 250}, {NOTE_F3, 250},  {NOTE_GS3, 500},
    {NOTE_F3, 375},  {NOTE_C4, 125},   {NOTE_A3, 500},  {NOTE_F3, 375},
    {NOTE_C4, 125},  {NOTE_A3, 1000}};

// Define the melodies
Melody melodies[] = {
    {superMarioNotes, sizeof(superMarioNotes) / sizeof(Note),
     "Super Mario Theme"},
    {starWarsIntroNotes, sizeof(starWarsIntroNotes) / sizeof(Note),
     "Starwars Melody"},
};

Melody getMelody(MelodyName name) { return melodies[name]; }
