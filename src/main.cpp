#include "webserver.hpp" // Просто должен быть первых (я хз лол)

#include "bindings.hpp"
#include "webview.h"
#include <nlohmann/json.hpp>
#include <string>

int main() {
  HttpServer server;
  server.start();

  webview::webview w(true, nullptr);
  w.set_title("Elysium Launcher");
  w.set_size(800, 600, WEBVIEW_HINT_NONE);

  MyBindings::registerRoutes(w);

#if USE_BUILTIN_WEBSERVER
  w.navigate(server.get_url());
#else
  w.navigate("http://localhost:5173/");
#endif

  w.run();
  return 0;
}
