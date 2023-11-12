#pragma once

#ifdef PLATFORM_ESP32
#include "ESP32/ADC.h"
#include "ESP32/DigitalIO.h"
#include "ESP32/PWM.h"
#include "ESP32/FileHandler.h"
#include "ESP32/Buzzer.h"
#elif defined(PLATFORM_NATIVE)
// For native testing, we will not include the real implementations here.
// Instead, the mocks will be included directly in the test files.
#endif