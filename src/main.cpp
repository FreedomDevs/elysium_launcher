#include "webserver.hpp"
#include "webview.h"

int main() {
  HttpServer server;
  server.start();

  webview::webview w(true, nullptr);
  w.set_title("My App");
  w.set_size(800, 600, WEBVIEW_HINT_NONE);
  w.navigate(server.get_url());
  w.run();
  return 0;
}
