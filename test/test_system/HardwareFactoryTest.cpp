#include "HardwareFactory.h"
#include "MockHardwareFactory.h"
#include "Mocks/MockADC.h"
#include "Mocks/MockFileHandler.h"
#include <gtest/gtest.h>

class HardwareFactoryTest : public ::testing::Test {
protected:
  std::shared_ptr<MockHardwareFactory> hardwareFactory;

  void SetUp() override {
    hardwareFactory = std::make_shared<MockHardwareFactory>();
  }
};

using ::testing::_;
using ::testing::ByMove;
using ::testing::NiceMock;
using ::testing::Return;

TEST_F(HardwareFactoryTest, CreateADCComponent) {
  HardwarePinConfig adcConfig{1, "VoltageSense", "ADC"};
  auto mockADC = std::unique_ptr<MockADC>(new MockADC(adcConfig));

  EXPECT_CALL(*hardwareFactory, createADC(adcConfig))
      .Times(1)
      .WillOnce(Return(ByMove(std::move(mockADC))));

  EXPECT_CALL(*hardwareFactory, createComponent(adcConfig))
      .Times(1)
      .WillOnce(testing::Invoke(hardwareFactory.get(), &HardwareFactory::createADC));

  auto adcComponent = hardwareFactory->createComponent(adcConfig);
  ASSERT_NE(adcComponent, nullptr);
}

TEST_F(HardwareFactoryTest, CreateFileHandler) {
  EXPECT_CALL(*hardwareFactory, createFileHandler())
      .Times(1)
      .WillOnce(Return(
          ByMove(std::unique_ptr<MockFileHandler>(new MockFileHandler()))));

  auto fileHandler = hardwareFactory->createFileHandler();
  ASSERT_NE(fileHandler, nullptr);
}

TEST_F(HardwareFactoryTest, CreateUnknownComponentType) {
  HardwarePinConfig unknownConfig{1, "XXX", "XXX"};

  auto component = hardwareFactory->createComponent(unknownConfig);

  ASSERT_EQ(component, nullptr);
}

TEST_F(HardwareFactoryTest, SingletonBehavior) {
  auto firstInstance = HardwareFactory::getHardwareFactory();
  auto secondInstance = HardwareFactory::getHardwareFactory();
  ASSERT_EQ(firstInstance, secondInstance);
}
