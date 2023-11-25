#pragma once

#ifdef PLATFORM_NATIVE

#include <IFileHandler.h>
#include <fstream>
#include <gmock/gmock.h>
#include <iostream>
#include <streambuf>

class MockFileHandlerWithRealLoad : public IFileHandler {
public:
  MOCK_METHOD(void, close, (), (override));
  MOCK_METHOD(bool, write, (const std::string &data), (override));
  MOCK_METHOD(bool, exists, (const std::string &path), (const, override));
  MOCK_METHOD(bool, remove, (const std::string &path), (override));

  // Method to open a real file
  bool open(const std::string &filePath, const std::string &mode) override {
    std::ifstream file(filePath);

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      realFileContent = std::string((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());
      file.close();
      return true;
    } catch (std::system_error &e) {
      std::cerr << e.code().message() << std::endl;
      return false;
    }
  }

  std::string read() override { return realFileContent; }

private:
  std::string realFileContent;
};

#endif // PLATFORM_NATIVE