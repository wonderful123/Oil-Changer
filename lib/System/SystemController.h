#include "ConfigManager.h"
#include "HardwareConfig.h"
#include "Logger.h"

class SystemController {
public:
  void initializeSystem();

private:
  ConfigManager configManager;
  void initializeHardware();
};