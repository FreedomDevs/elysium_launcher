#pragma once

#include "httplib.h"
#include <chrono>
#include <filesystem>
#include <random>
#include <stdexcept>
#include <string>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

class HttpServer {
public:
  explicit HttpServer(const std::string &root_path = "./build/dist/")
      : root_(std::filesystem::absolute(root_path).string()), svr_() {
    init_sockets();
    port_ = find_free_port();
    host_ = "127.0.0.1"; // слушаем локально (loopback)
    svr_.set_mount_point("/", root_.c_str());
  }

  ~HttpServer() {
    stop();
    cleanup_sockets();
  }

  void start() {
    server_thread_ =
        std::thread([this]() { svr_.listen(host_.c_str(), port_); });
  }

  void stop() {
    svr_.stop();
    if (server_thread_.joinable())
      server_thread_.join();
  }

  std::string get_url() const {
    return "http://" + host_ + ":" + std::to_string(port_);
  }

private:
  std::string root_;
  std::string host_;
  int port_;
  httplib::Server svr_;
  std::thread server_thread_;

  static void init_sockets() {
#ifdef _WIN32
    static bool initialized = false;
    if (!initialized) {
      WSADATA wsaData;
      if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        throw std::runtime_error("WSAStartup failed");
      initialized = true;
    }
#endif
  }

  static void cleanup_sockets() {
#ifdef _WIN32
    WSACleanup();
#endif
  }

  static bool is_port_free(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
      return false;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(port);

    int opt = 1;
#ifdef _WIN32
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const char *>(&opt), sizeof(opt));
#else
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    int result = bind(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));

#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif

    return (result == 0);
  }

  static int find_free_port() {
    std::mt19937 rng(
        std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(49152, 65535);

    for (int i = 0; i < 100; ++i) {
      int port = dist(rng);
      if (is_port_free(port)) {
        return port;
      }
    }
    throw std::runtime_error("Cannot find free port");
  }
};
