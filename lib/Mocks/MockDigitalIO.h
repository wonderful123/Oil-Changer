#include "IDigitalIO.h"
#include <iostream>
#include <string>

#ifdef PLATFORM_NATIVE
#ifndef LOW
#define LOW 0
#endif

#ifndef HIGH
#define HIGH 1
#endif

class MockDigitalIO : public IDigitalIO {
public:
  explicit MockDigitalIO(const GpioPinConfig &config)
      : IDigitalIO(config), _mockValue(LOW) {
    std::string modeStr = "OUTPUT"; // Default mode
    auto it = config.options.find("mode");
    if (it != config.options.end()) {
      modeStr = it->second;
    }
    
    _mode = modeStr == "INPUT" ? INPUT : OUTPUT;

    std::cout << "MockDigitalIO initialized on pin: " << _pinNumber
              << " with mode: " << (_mode == INPUT ? "INPUT" : "OUTPUT")
              << std::endl;
    setInitialized(true); // Mark MockDigitalIO as initialized
  }

  virtual int read() const override {
    if (!isInitialized()) {
      std::cerr << "MockDigitalIO: Attempted to read uninitialized DigitalIO."
                << std::endl;
      return -1; // or some error code
    }
    // Return a simulated digital IO value
    return _mockValue;
  }

  virtual void write(int value) override {
    if (!isInitialized()) {
      std::cerr
          << "MockDigitalIO: Attempted to write to uninitialized DigitalIO."
          << std::endl;
      return;
    }
    _mockValue = value; // Simulate writing a value
    std::cout << "MockDigitalIO: Pin " << _pinNumber << " set to "
              << (_mockValue == HIGH ? "HIGH" : "LOW") << std::endl;
  }

  virtual Mode getMode() const override { return _mode; }

  // Additional methods for testing
  void setMockValue(int value) { _mockValue = value; }

private:
  int _mockValue; // Simulated Digital IO value
  Mode _mode;     // Mode of the Digital IO
};

#endif // PLATFORM_NATIVE
