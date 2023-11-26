#include "HardwareFactory.h"
#include "MockHardwareFactory.h"
#include <gtest/gtest.h>

class HardwareFactoryTest : public ::testing::Test {
protected:
  std::shared_ptr<HardwareFactory> hardwareFactory;
  std::shared_ptr<MockHardwareFactory> mockHardwareFactory;

  void SetUp() override {
    mockHardwareFactory = std::make_shared<MockHardwareFactory>();
    hardwareFactory = HardwareFactory::getHardwareFactory();
  }
};

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;
