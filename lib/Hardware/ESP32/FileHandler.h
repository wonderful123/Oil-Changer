#pragma once

#ifdef PLATFORM_ESP32

#include "IFileHandler.h"
#include <LittleFS.h>

/**
 * @class FileHandler
 * @brief LittleFS-based file handling for the ESP32 platform.
 *
 * Provides an implementation of the IFileHandler interface using the LittleFS
 * file system on ESP32.
 */
class FileHandler : public IFileHandler {
public:
  FileHandler();

  /**
   * @brief Opens a file with a specified mode.
   *
   * @param mode Specifies the file mode (e.g., "r" for reading, "w" for
   * writing).
   * @return true If the file was opened successfully.
   * @return false If the file opening failed.
   */
  bool open(const std::string &filePath, const std::string &mode) override;

  /**
   * @brief Writes data to the file.
   *
   * @param data Data to write to the file.
   * @return true If data was written successfully.
   * @return false If data writing failed.
   */
  bool write(const std::string &data) override;

  /**
   * @brief Reads content from the file.
   *
   * @return std::string Content read from the file.
   */
  std::string read() override;

  /**
   * @brief Checks if the file exists in the filesystem.
   *
   * @return true If the file exists.
   * @return false If the file doesn't exist.
   */
  bool exists(const std::string &filePath) const override;

  /**
   * @brief Closes the opened file.
   */
  void close() override;

  /**
   * @brief Deletes the file.
   *
   * @return true If the file was deleted successfully.
   * @return false If file deletion failed.
   */
  bool remove(const std::string &filePath) override;

private:
  File _file;
  std::string _currentFilePath;
  bool _isFileSystemMounted;
};

#endif // PLATFORM_ESP32
