// Event.h
#pragma once

enum class Event { Buzzer, OilChangeTracker, Motor, Display, State };

enum class Parameter {
  // Buzzer Parameters
  SingleBeep,
  RapidBeep,
  DoubleBeep,
  StopBeep,

  // OilChangeTracker Parameters
  FillCapacity,
  Reset, // For starting a new fill

  // Motor Parameters
  MotorFill,
  MotorExtract,
  MotorStop, // stop motor with ramping if enabled
  MotorHalt, // stop motor right away

  // Display Parameters
  DisplayMessage,
  SecondaryDisplayMessage,

  // Current FSM states
  Initializing,
  Ready,
  ManualFill,
  ManualExtract,
  Extracting,
  InterimTask,
  Filling
};