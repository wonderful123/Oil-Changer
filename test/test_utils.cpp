#include "test_utils.h"

void expectOpenReadCloseForContent(MockFileHandler &mockFileHandler,
                                   const std::string &content,
                                   const std::string &filePath) {
  EXPECT_CALL(mockFileHandler, open(filePath, "r"))
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(mockFileHandler, read()).WillOnce(::testing::Return(content));
  EXPECT_CALL(mockFileHandler, close()).Times(1);
}
