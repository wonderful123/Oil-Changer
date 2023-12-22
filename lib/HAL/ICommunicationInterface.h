#pragma once

#include <string>

class ICommunicationInterface {
 public:
  virtual ~ICommunicationInterface() = default;

  virtual void send(const std::string &message) = 0;
  virtual std::string receive() = 0;  // Depending on your needs
};
