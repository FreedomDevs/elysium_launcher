#pragma once

#include "webview.h"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace MyBindings {

inline std::string pingCpp(const std::string &req) {
  nlohmann::json j;
  j["status"] = "ok";
  return j.dump();
}

const std::unordered_map<std::string,
                         std::function<std::string(const std::string &)>>
    routeMap = {
        {"pingCpp", pingCpp},
};

inline void registerRoutes(webview::webview &w) {
  for (const auto &[route, handler] : routeMap) {
    w.bind(route, handler);
  }
}
} // namespace MyBindings
