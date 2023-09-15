#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <linux/un.h>
#include <list>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include "bdb_errors.hpp"

/*!
 * @class Bdb socket server class
 * https://gist.github.com/alexandruc/2350954
 * https://www.ibm.com/docs/en/ztpf/1.1.0.15?topic=considerations-unix-domain-sockets
 */
class Bdb_inet_socket_server {
 private:
  int backlog{10};
  std::string filename{};
  bool opened{};
  int port{};
  int server_socket{};
  void close_socket();
  void open_socket(Bdb_errors &errors);
  struct sockaddr_in server_inet_sockaddr{};
 public:
  ~Bdb_inet_socket_server();
  Bdb_inet_socket_server(int port_, Bdb_errors &errors);
  [[nodiscard]] int get_server_socket() const { return server_socket; }
  static std::string http_body(char *buffer, int bytesRead);
  static std::string http_header(const std::string &request);
  [[nodiscard]] bool is_open() const { return opened; }

  class Bdb_inet_socket_client {
   private:
    char client_ip[INET_ADDRSTRLEN] = {0};
    struct sockaddr_in client_inet_sockaddr{};
    int client_socket{};
    bool opened{};
    void close_socket();
    void open_socket(Bdb_inet_socket_server &bdb_socket_server, Bdb_errors &errors);

   public:
    ~Bdb_inet_socket_client();
    Bdb_inet_socket_client(Bdb_inet_socket_server &bdb_socket_server, Bdb_errors &errors);
    int read_socket(char *buffer, size_t buffer_len, Bdb_errors &errors) const;
    void write_socket(const char *buffer, size_t buffer_len, Bdb_errors &errors) const;
  };
};

