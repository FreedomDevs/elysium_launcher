#include "webview.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

std::string extract_first_string_arg(const std::string &json_array_str) {
  try {
    auto json = nlohmann::json::parse(json_array_str);
    if (json.is_array() && !json.empty() && json[0].is_string()) {
      return json[0].get<std::string>();
    }
  } catch (...) {
    // Игнорируем любые ошибки
  }
  return "";
}

std::string json_escape(const std::string &s) {
  return nlohmann::json(s).dump();
}
int main() {
  webview::webview w(true, nullptr);
  w.set_title("WebView");
  w.set_size(800, 600, WEBVIEW_HINT_NONE);

  w.bind("cppFunction", [](const std::string &req) -> std::string {
    std::string msg = extract_first_string_arg(req);
    std::cout << "Из JS пришло: " << msg << std::endl;

    std::string reply = "Ответ от C++: Получено '" + msg + "'";
    return json_escape(reply);
  });

  const char *html = R"HTML(
    <!doctype html>
    <html>
      <body>
        <h1>WebView</h1>
        <button onclick="callCpp()">Вызвать C++</button>
        <p id="result"></p>
        <script>
          async function callCpp() {
            try {
              let result = await window.cppFunction("Привет от JS!");
              document.getElementById("result").innerText = result;
            } catch (e) {
              console.error("Ошибка:", e);
            }
          }
          window.onload = () => {
            console.log("JS: Страница загружена");
          };
        </script>
      </body>
    </html>
  )HTML";

  w.set_html(html);
  w.run();
  return 0;
}
