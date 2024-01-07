// Event.h
#pragma once

enum class Event {
  Buzzer,
  OilChangeTracker,
  Motor,
};

enum class Parameter {
  // Buzzer Parameters
  SingleBeep,
  RapidBeep,
  DoubleBeep,
  StopBeep,

  // OilChangeTracker Parameters
  FillCapacity,

  // Motor Parameters
  MotorFill,
  MotorExtract,
  MotorStop, // stop motor with ramping if enabled
  MotorHalt, // stop motor right away
};