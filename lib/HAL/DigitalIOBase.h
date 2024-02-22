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
  virtual void setMode(Mode mode) override { _mode = mode; }

protected:
  void setPinModeFromConfig(const HardwarePinConfig &config) {
    std::string modeStr = config.getOptionAs<std::string>("mode");

    static const std::unordered_map<std::string, Mode> modeMap = {
        {"Input", Mode::MODE_INPUT},
        {"Output", Mode::MODE_OUTPUT},
        {"PullUp", Mode::MODE_PULLUP},
        {"PullDown", Mode::MODE_PULLDOWN}};

    auto it = modeMap.find(modeStr);
    if (it != modeMap.end()) {
      _mode = it->second;
    } else {
      Error(Error::DigitalIOModeOptionMissingOrInvalid);
      _mode = Mode::MODE_INPUT; // Defaulting to MODE_INPUT
    }
  }

  IDigitalIO::Mode _mode; // Mode of the Digital IO (INPUT/OUTPUT)
};
