#pragma once
#include "mocks/mock_FileHandler.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

void expectOpenReadCloseForContent(MockFileHandler &mockFileHandler,
                                   const std::string &content,
                                   const std::string &filePath);
