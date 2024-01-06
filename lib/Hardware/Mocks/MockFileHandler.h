// MockFileHandler.h
#pragma once

#ifdef PLATFORM_NATIVE

#include "IFileHandler.h"
#include <gmock/gmock.h>
#include <string>

class MockFileHandler : public IFileHandler {
public:
  MOCK_METHOD(bool, open, (const std::string &, const std::string &),
              (override));
  MOCK_METHOD(void, close, (), (override));
  MOCK_METHOD(std::string, read, (), (override));
  MOCK_METHOD(bool, write, (const std::string &data), (override));
  MOCK_METHOD(bool, exists, (const std::string &path), (const, override));
  MOCK_METHOD(bool, remove, (const std::string &path), (override));
};

#endif // PLATFORM_NATIVE