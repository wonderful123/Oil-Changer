// DigitalIOBase.h
#pragma once

#include "Error.h"
#include "IDigitalIO.h"

class DigitalIOBase : public IDigitalIO {
public:
  explicit DigitalIOBase(const HardwarePinConfig &config) : IDigitalIO(config) {
    setPinModeFromConfig(config);
    setInitialized(true);
  }

  // Override the destructor
  virtual ~DigitalIOBase() override = default;
  virtual int read() const override = 0;
  virtual void write(int value) override = 0;

  virtual Mode getMode() const override { return _mode; }

protected:
  void setPinModeFromConfig(const HardwarePinConfig &config) {
    std::string mode = config.getOptionAs<std::string>("mode");

    if (mode == "INPUT") {
      _mode = IDigitalIO::Mode::INPUT_MODE;
    } else if (mode == "OUTPUT") {
      _mode = IDigitalIO::Mode::OUTPUT_MODE;
    } else {
      // Handle invalid mode
      Error(Error::DigitalIOModeOptionMissingOrInvalid);
      _mode = IDigitalIO::Mode::INPUT_MODE; // Default to INPUT mode
    }
  }

  IDigitalIO::Mode _mode; // Mode of the Digital IO (INPUT/OUTPUT)
};
