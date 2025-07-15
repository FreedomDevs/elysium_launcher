#include "httplib.h"
#include "webview.h"
#include <filesystem>

int main() {
  using namespace httplib;
  Server svr;

  std::string root = std::filesystem::absolute("./build/dist/").string();

  svr.set_mount_point("/", root.c_str());

  std::thread server_thread([&]() { svr.listen("localhost", 8080); });

  webview::webview w(true, nullptr);
  w.set_title("My App");
  w.set_size(800, 600, WEBVIEW_HINT_NONE);
  w.navigate("http://localhost:8080");
  w.run();

  svr.stop();
  server_thread.join();

  return 0;
}
