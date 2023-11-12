#pragma once

#include <string>

/**
 * @interface IFileHandler
 * @brief Interface for generic file handling operations.
 *
 * This interface provides common file operations like opening, reading,
 * writing, checking existence, closing, and deleting files. Implementations are
 * expected to provide the specific functionality for the targeted platform or
 * file system.
 */
class IFileHandler {
public:
  virtual ~IFileHandler() = default;

  /**
   * @brief Opens the file with the given file path and mode.
   *
   * @param filePath The path to the file.
   * @param mode The mode to open the file in (e.g., "r", "w", "a").
   * @return true If the file was opened successfully.
   * @return false Otherwise.
   */
  virtual bool open(const std::string &filePath, const std::string &mode) = 0;

  /**
   * @brief Writes data to the file.
   *
   * @param data The data to write to the file.
   * @return true If the write operation was successful.
   * @return false Otherwise.
   */
  virtual bool write(const std::string &data) = 0;

  /**
   * @brief Reads data from the open file.
   *
   * @return std::string The data read from the file.
   */
  virtual std::string read() = 0;

  /**
   * @brief Checks if the file exists at the given file path.
   *
   * @param filePath The path to the file to check for existence.
   * @return true If the file exists at the given path.
   * @return false Otherwise.
   */
  virtual bool exists(const std::string &filePath) const = 0;

  /**
   * @brief Closes the currently open file.
   *
   * Implementations should ensure any buffered data is written and resources
   * are released.
   */
  virtual void close() = 0;

  /**
   * @brief Deletes the file at the given file path.
   *
   * @param filePath The path to the file to delete.
   * @return true If the file was deleted successfully.
   * @return false Otherwise.
   */
  virtual bool remove(const std::string &filePath) = 0;
};
