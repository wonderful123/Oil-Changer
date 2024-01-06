// DisplayConfig.h
#pragma once

class DisplayConfig {
public:
  std::string id;
  std::string interfaceId;

  DisplayConfig(const std::string &id, const std::string &interfaceId)
      : id(id), interfaceId(interfaceId) {}
};