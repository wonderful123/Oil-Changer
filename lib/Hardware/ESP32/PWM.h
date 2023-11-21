#pragma once

#include "PWMBase.h"

#ifdef PLATFORM_ESP32

/**
 * @class PWM
 * @brief ESP32-specific implementation of PWM functionality.
 */
class PWM : public PWMBase {
public:
  /**
   * @brief Construct a new PWM object for ESP32.
   *
   * @param config Configuration for the PWM pin.
   */
  PWM(const GpioPinConfig &config);

protected:
  /**
   * @brief Apply the specified duty cycle to the PWM signal.
   *
   * @param dutyCycle Duty cycle to be set (range: 0.0 to 1.0).
   */
  virtual void applyDutyCycle(double dutyCycle) override;

  /**
   * @brief Set the frequency of the PWM signal.
   *
   * @param frequency Frequency in Hertz.
   */
  virtual void applyFrequency(double frequency) override;

private:
  /**
   * @brief Initialize the PWM configuration.
   */
  void initializePWM();

  static constexpr int MAX_PWM_CHANNEL =
      15; // ESP32 supports 16 channels (0 to 15)
  static constexpr int PWM_RESOLUTION = 8; // 8-bit resolution
};

#endif // PLATFORM_ESP32
