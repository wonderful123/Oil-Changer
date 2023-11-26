#include "HardwareFactory.h"
#include "MockHardwareFactory.h"
#include <gtest/gtest.h>

class HardwareFactoryTest : public ::testing::Test {
protected:
  std::shared_ptr<HardwareFactory> hardwareFactory =
      HardwareFactory::getHardwareFactory();
};

using ::testing::_;
using ::testing::Invoke;

// Test case for creating ADC component
TEST_F(HardwareFactoryTest, CreateADCComponent) {
  HardwarePinConfig config(1, "VoltageSense", "ADC");
  auto adcComponent = hardwareFactory->createComponent(config);
  ASSERT_NE(adcComponent, nullptr);
  ASSERT_EQ(adcComponent->type(), "ADC");
  ASSERT_EQ(adcComponent->id(), "VoltageSense");
  ASSERT_EQ(adcComponent->pinNumber(), 1);
}

TEST_F(HardwareFactoryTest, CreateBuzzerComponent) {
  HardwarePinConfig config(12, "Buzzer", "Buzzer");
  auto buzzerComponent = hardwareFactory->createComponent(config);
  ASSERT_NE(buzzerComponent, nullptr);
  ASSERT_EQ(buzzerComponent->type(), "Buzzer");
  ASSERT_EQ(buzzerComponent->id(), "Buzzer");
  ASSERT_EQ(buzzerComponent->pinNumber(), 12);
}

TEST_F(HardwareFactoryTest, CreateFlowMeterFillComponent) {
  HardwarePinConfig config(14, "FlowMeterFill", "FlowMeter");
  config.options["pcntUnit"] = "PCNT_UNIT_0";

  auto flowMeterFillComponent = hardwareFactory->createComponent(config);

  ASSERT_NE(flowMeterFillComponent, nullptr);
  ASSERT_EQ(flowMeterFillComponent->type(), "FlowMeter");
  ASSERT_EQ(flowMeterFillComponent->id(), "FlowMeterFill");
  ASSERT_EQ(flowMeterFillComponent->pinNumber(), 14);
}

TEST_F(HardwareFactoryTest, CreateSPIComponent) {
  std::unordered_map<std::string, int> spiPins = {
      {"MOSI", 23},
      {"MISO", 19},
      {"SCLK", 18},
      {"CS", 5},
  };
  HardwarePinConfig config(spiPins, "SPI", "SPI");
  auto spiComponent = hardwareFactory->createComponent(config);
  ASSERT_NE(spiComponent, nullptr);
  ASSERT_EQ(spiComponent->type(), "SPI");
  ASSERT_EQ(spiComponent->id(), "SPI");
}

TEST_F(HardwareFactoryTest, ReturnsNullptrForUnknownComponentType) {
  HardwarePinConfig unknownConfig{99, "Unknown", "UnknownType"};
  auto unknownComponent = hardwareFactory->createComponent(unknownConfig);
  EXPECT_EQ(unknownComponent, nullptr);
}
