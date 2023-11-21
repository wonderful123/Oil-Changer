#pragma once

#include "Mocks/MockFileHandler.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

    void
    expectOpenReadCloseForContent(MockFileHandler &mockFileHandler,
                                  const std::string &content,
                                  const std::string &filePath);
