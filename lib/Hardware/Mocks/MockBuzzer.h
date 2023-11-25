#pragma once

#ifdef PLATFORM_NATIVE

#include "IBuzzer.h"
#include <iostream>
#include <string>

/**
 * @class Buzzer
 * @brief Mock class for Buzzer, adhering to the IBuzzer interface.
 *
 * This mock class is used for unit testing in a native environment, allowing
 * simulation of buzzer behavior and setting expectations on its methods.
 */
class MockBuzzer : public IBuzzer {
public:
  explicit MockBuzzer(const HardwarePinConfig &config) : IBuzzer(config) {
    // Simulate hardware initialization for testing purposes
    std::cout << "MockBuzzer initialized on pin: " << _pinNumber << std::endl;

    setInitialized(true); // Mark MockBuzzer as initialized
  }

  void beep(int frequency, int duration) override {
    std::cout << "MockBuzzer::beep called with frequency: " << frequency
              << ", duration: " << duration << std::endl;
    // You can add more sophisticated logic here if needed for testing
  }

  void stop() override {
    std::cout << "MockBuzzer::stop called" << std::endl;
    // Implement any additional logic required for testing
  }

  void setVolume(float volume) override {
    std::cout << "MockBuzzer::setVolume called with volume: " << volume
              << std::endl;
    // Store volume or perform additional logic for testing
  }

  MOCK_METHOD(bool, isBeeping, (), (const, override));
};

#endif // PLATFORM_NATIVE
