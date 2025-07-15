#include "webserver.hpp"
#include "webview.h"
#include <nlohmann/json.hpp>
#include <string>

int main() {
  HttpServer server;
  server.start();

  webview::webview w(true, nullptr);
  w.set_title("My App");
  w.set_size(800, 600, WEBVIEW_HINT_NONE);

  w.bind("pingCpp", [](const std::string &req) -> std::string {
    nlohmann::json j;
    j["status"] = "ok";
    return j.dump();
  });

#if USE_BUILTIN_WEBSERVER
  w.navigate(server.get_url());
#else
  w.navigate("http://localhost:5173/");
#endif

  w.run();
  return 0;
}
