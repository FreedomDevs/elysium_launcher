#include "webserver.hpp" // Просто должен быть первым (я хз лол)

#include "bindings.hpp"
#include "webview.h"
#include <nlohmann/json.hpp>
#include <string>

extern "C" {
#include "el-core-bindings.h"
}

int main() {
  HttpServer server;
  server.start();

  std::cout << rust_add(32, 32) << std::endl;
  webview::webview w(true, nullptr);
  w.set_title("Elysium Launcher");
  w.set_size(800, 600, WEBVIEW_HINT_NONE);
  MyBindings::registerRoutes(w);

#if USE_BUILTIN_WEBSERVER
  std::cout << "Server started at: " << server.get_url() << std::endl;
  w.navigate(server.get_url());
#else
  std::cout << "Running on http://localhost:5173" << std::endl;
  w.navigate("http://localhost:5173/");
#endif

  w.run();
  return 0;
}
