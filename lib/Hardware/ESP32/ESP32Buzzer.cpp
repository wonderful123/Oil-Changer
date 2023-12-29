#ifdef PLATFORM_ESP32

#include "ESP32Buzzer.h"

const uint pwmChannel = 0;
const uint pwmResolution = 8;
const uint pwmFrequency = 2713;

ESP32Buzzer::ESP32Buzzer(const HardwarePinConfig &config)
    : BuzzerBase(config), _isBeeping(false), _isRapidBeeping(false),
      _isSecondBeep(false), _beepFrequency(0), _beepDuration(0) {
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(config.pinNumber, pwmChannel);
  setInitialized(true);
}

ESP32Buzzer::~ESP32Buzzer() {
  stop(); // Ensure the buzzer is stopped
}

void ESP32Buzzer::setVolume(float volume) {
  BuzzerBase::setVolume(volume); // Store the volume in the base class
  int dutyCycle = static_cast<int>(
      _volume * 255.0f); // Convert volume to duty cycle (0-255)
  ledcWrite(pwmChannel,
            dutyCycle); // Adjust the duty cycle to change the volume
  ledcWriteTone(pwmChannel, 0);
}

void ESP32Buzzer::beep(uint frequency, uint duration) {
  ledcWriteTone(pwmChannel, frequency);
  _isBeeping = true;
  _timer.once_ms(duration, timerCallback, this);
}

void ESP32Buzzer::timerCallback(ESP32Buzzer *buzzer) {
  ledcWriteTone(pwmChannel, 0);
  buzzer->_isBeeping = false;
}

void ESP32Buzzer::doubleBeep(uint frequency, uint duration,
                             uint pauseDuration) {
  _beepFrequency = frequency;
  _beepDuration = duration;
  _isSecondBeep = false;     // Indicates the first beep is being played
  beep(frequency, duration); // Start the first beep
  // Schedule the second beep after the first beep duration plus the pause
  // duration
  _rapidBeepTimer.once_ms(duration + pauseDuration, &doubleBeepCallback, this);
}

void ESP32Buzzer::doubleBeepCallback(ESP32Buzzer *buzzer) {
  if (buzzer) {
    if (!buzzer->_isSecondBeep) {
      // Time for the second beep
      buzzer->_isSecondBeep = true;
      buzzer->beep(buzzer->_beepFrequency, buzzer->_beepDuration);
    } else {
      // Second beep is done, stop beeping
      buzzer->stop();
    }
  }
}

void ESP32Buzzer::rapidBeep(uint frequency, uint duration, uint pauseInterval) {
  _rapidBeepFrequency = frequency;
  _rapidBeepDuration = duration;
  _rapidBeepPauseInterval = pauseInterval;
  _isRapidBeeping = true;

  beep(frequency, duration);
  _rapidBeepTimer.once_ms(duration + pauseInterval, &rapidBeepCallback, this);
}

void ESP32Buzzer::rapidBeepCallback(ESP32Buzzer *buzzer) {
  if (buzzer && buzzer->_isRapidBeeping) {
    buzzer->beep(buzzer->_rapidBeepFrequency, buzzer->_rapidBeepDuration);
    // Schedule the next beep after the current beep duration PLUS the pause
    // interval.
    buzzer->_rapidBeepTimer.once_ms(buzzer->_rapidBeepDuration +
                                        buzzer->_rapidBeepPauseInterval,
                                    rapidBeepCallback, buzzer);
  }
}

void ESP32Buzzer::stop() {
  if (_isBeeping) {
    ledcWriteTone(pwmChannel, 0); // Stop the buzzer sound instantly
    _timer.detach();              // Detach the timer to stop callbacks
    _isBeeping = false;
  }
  if (_isRapidBeeping) {
    _isRapidBeeping = false;
    _rapidBeepTimer.detach();
  }
}

bool ESP32Buzzer::isBeeping() const { return _isBeeping; }

#endif // PLATFORM_ESP32
