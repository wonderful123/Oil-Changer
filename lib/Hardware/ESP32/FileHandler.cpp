#ifdef PLATFORM_ESP32

#include "FileHandler.h"
#include "Error.h"
#include "FS.h"
#include "Logger.h"
#include <LittleFS.h>

FileHandler::FileHandler() {
  _isFileSystemMounted = LittleFS.begin();
  if (!_isFileSystemMounted) {
    // Log the error using the Error class
    Error(Error::FileSystemMountFailure);
  }
}

bool FileHandler::open(const std::string &filePath, const std::string &mode) {
  if (_file) {
    _file.close(); // Close any previously opened file
  }

  _currentFilePath = filePath; // Store the current file path

  _file = LittleFS.open(filePath.c_str(), mode.c_str());
  if (!_file) {
    Error(Error::FileOpenFailure);
    return false;
  }
  return true;
}

bool FileHandler::write(const std::string &data) {
  if (!_file) {
    Logger::warn("Write attempted on unopened file");
    return false;
  }
  size_t bytesWritten = _file.print(data.c_str());
  if (bytesWritten != data.length()) {
    Error(Error::FileWriteFailure);
    return false;
  }
  return true;
}

std::string FileHandler::read() {
  std::string content;
  if (!_file) {
    Logger::warn("Read attempted on unopened file");
    return content;
  }
  while (_file.available()) {
    content += static_cast<char>(_file.read());
  }
  return content;
}

bool FileHandler::exists(const std::string &filePath) const {
  return LittleFS.exists(filePath.c_str());
}

void FileHandler::close() {
  if (_file) {
    _file.close();
  }
  _currentFilePath.clear();
}

bool FileHandler::remove(const std::string &filePath) {
  if (!LittleFS.exists(filePath.c_str())) {
    Logger::warn("Attempt to remove non-existing file: " + filePath);
    return false;
  }
  return LittleFS.remove(filePath.c_str());
}

#endif // PLATFORM_ESP32
