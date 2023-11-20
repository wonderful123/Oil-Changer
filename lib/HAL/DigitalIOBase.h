#pragma once

#include "Error.h"
#include "IDigitalIO.h"

class DigitalIOBase : public IDigitalIO {
public:
  explicit DigitalIOBase(const GpioPinConfig &config) : IDigitalIO(config) {
    setPinModeFromConfig(config);
    setInitialized(true);
  }

  // Override the destructor
  virtual ~DigitalIOBase() override = default;
  virtual int read() const override = 0;
  virtual void write(int value) override = 0;

  virtual Mode getMode() const override { return _mode; }

protected:
  void setPinModeFromConfig(const GpioPinConfig &config) {
    std::string mode = config.getOptionAs<std::string>("mode");

    if (mode == "INPUT") {
      _mode = IDigitalIO::INPUT_MODE;
    } else if (mode == "OUTPUT") {
      _mode = IDigitalIO::OUTPUT_MODE;
    } else {
      // Handle invalid mode
      Error(DigitalIOModeOptionMissingOrInvalid);
      _mode = IDigitalIO::INPUT_MODE; // Default to INPUT mode
    }
  }

  IDigitalIO::Mode _mode; // Mode of the Digital IO (INPUT/OUTPUT)
};
