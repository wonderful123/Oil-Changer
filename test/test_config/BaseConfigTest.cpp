#include "Error.h"
#include "IFileHandler.h"
#include "Mocks/MockFileHandler.h"
#include "test_utils.h"
#include <BaseConfig.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

/************************** Test Summary ****************************/
/*

  Basic Functionality Tests:
    - LoadFailure: Ensures error handling for non-existent file loading.
    - JSONParseFailure: Validates error response to malformed JSON content.

*/
/*******************************************************************/

class DummyBaseConfig : public BaseConfig {
public:
  explicit DummyBaseConfig(std::shared_ptr<IFileHandler> fileHandler)
      : BaseConfig(std::move(fileHandler)) {}

  Error save(const std::string &filename) const override {
    // Placeholder implementation
    return Error::OK;
  }

  Error parseJson(const DynamicJsonDocument &doc) override {
    // Placeholder implementation
    return Error::OK;
  }
};

class BaseConfigTest : public ::testing::Test {
protected:
  std::shared_ptr<MockFileHandler> mockFileHandler;
  std::shared_ptr<DummyBaseConfig> dummyBaseConfig;
  const std::string TEST_FILE_PATH = "test_config.json";

  BaseConfigTest() {
    mockFileHandler = std::make_shared<MockFileHandler>();
    dummyBaseConfig = std::make_shared<DummyBaseConfig>(mockFileHandler);
  }
};

TEST_F(BaseConfigTest, LoadFailure) {
  EXPECT_CALL(*mockFileHandler, open(TEST_FILE_PATH, "r"))
      .WillOnce(::testing::Return(false)); // Simulate file open failure

  Error result = dummyBaseConfig->load(TEST_FILE_PATH);
  EXPECT_EQ(result,
            Error::FileOpenFailure); // Expect specific error for file not open
}

TEST_F(BaseConfigTest, JSONParseFailure) {
  const std::string invalidMockFileContent =
      R"({ "someKey": "someValue", xxx })";

  expectOpenReadCloseForContent(*mockFileHandler, invalidMockFileContent,
                                TEST_FILE_PATH);

  Error result = dummyBaseConfig->load(TEST_FILE_PATH);
  EXPECT_EQ(result, Error::JsonInputInvalid)
      << result.getFormattedMessage(result.code()); // Expect JSON parse error
}