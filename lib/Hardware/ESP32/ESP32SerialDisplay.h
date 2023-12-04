#include "IDisplay.h"
#include "ISerial.h"
#include <string>

class SerialDisplay : public IDisplay {
private:
  ISerial &_serialInterface;

public:
  // Constructor
  explicit SerialDisplay(ISerial &serialInterface)
      : _serialInterface(serialInterface) {
    _serialInterface.begin(
        115200); // Initialize Serial communication with desired baud rate
  }

  // Destructor
  virtual ~SerialDisplay() {
    _serialInterface.end(); // Clean up Serial communication
  }

  // Override displayData to output to Serial
  virtual void displayData(const std::string &data) override {
    for (char c : data) {
      _serialInterface.write(static_cast<uint8_t>(c));
    }
    _serialInterface.write(static_cast<uint8_t>('\n')); // For newline
  }

  void update(EventType eventType) {
    // Implementation for handling the update event
  }
};
