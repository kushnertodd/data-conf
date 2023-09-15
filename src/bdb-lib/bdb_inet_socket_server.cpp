#include <cerrno>
#include <cstring>
#include <iostream>
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "bdb_inet_socket_server.hpp"

/*!
 * @brief close server socket and delete unix domain socket file when destructed
 * @details invariant: opened = socket is open
 */
Bdb_inet_socket_server::~Bdb_inet_socket_server() {
  close_socket();
}

/*!
 * @brief construct and open server socket 
 * @param filename_ unix domain socket filename
 * @param errors if open errors
 * @details invariant: opened = socket is open
 */
//Bdb_inet_socket_server::Bdb_inet_socket_server(Bdb_errors &errors) :
//    Bdb_inet_socket_server(Cerver_socket_filename, errors) {
//}

Bdb_inet_socket_server::Bdb_inet_socket_server(int port_, Bdb_errors &errors) :
    port(port_) {
  open_socket(errors);
}

/*!
 * @brief ensure server socket is closed and unix domain socket file is deleted
 * @details invariant: opened = socket is open
 */
void Bdb_inet_socket_server::close_socket() {
  // Close the server socket
  if (opened) {
    close(server_socket);
    opened = false;
    std::cout << "Bdb_inet_socket_server::close_socket: server socket " << port
              << " closed (" << server_socket
              << ")" << std::endl;
  }
}

std::string Bdb_inet_socket_server::http_body(char *buffer, int bytesRead) {
  std::string requestData(buffer, bytesRead);
  size_t headerEnd = requestData.find("\r\n\r\n");
  if (headerEnd == std::string::npos) {
    std::cerr << "Invalid request. Header not found.\n";
    return "";
  }
  // Strip off the header and extract the request body
  return requestData.substr(headerEnd + 4);
}

std::string Bdb_inet_socket_server::http_header(const std::string &request) {
  size_t size = request.size();
  return
      "HTTP/1.1 200 OK\n"
      "Content-Type: application/json\n"
      "Access-Control-Allow-Origin: *\n"
      "Content-Length: " + std::to_string(size) + "\n\n"
          + request;
}

/*!
 * @brief open server socket
 * @param errors if open errors
 * @details invariant: opened = socket is open
 */
void Bdb_inet_socket_server::open_socket(Bdb_errors &errors) {
  // only called from constructor (once)
  opened = false;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
    errors.add("Bdb_inet_socket_server::open_socket", "1",
               "Failed to create socket", errno);
  else {
    opened = true;
    std::cout << "Bdb_inet_socket_server::open_socket: server socket opened ("
              << server_socket << ")"
              << std::endl;
    memset(&server_inet_sockaddr, 0, sizeof(server_inet_sockaddr));
    // Set up the server address
    server_inet_sockaddr.sin_family = AF_INET;
    server_inet_sockaddr.sin_addr.s_addr = INADDR_ANY;
    server_inet_sockaddr.sin_port = htons(port);
    // Bind the socket to the server address
    if (bind(server_socket, reinterpret_cast<sockaddr *>(&server_inet_sockaddr),
             sizeof(server_inet_sockaddr)) == -1)
      errors.add("Bdb_inet_socket_server::open_socket", "2",
                 "Failed to bind socket " + std::to_string(port) + " (" +
                     std::to_string(server_socket) + ")", errno);
  }
  if (!errors.has()) {
    // Listen for incoming connections
    if (listen(server_socket, backlog) == -1)
      errors.add("Bdb_inet_socket_server::open_socket", "3",
                 "Failed to listen for connections", errno);
  }
}

/*
 * @brief ensure server socket is closed
 * @details invariant: opened = socket is open
 */
Bdb_inet_socket_server::Bdb_inet_socket_client::~Bdb_inet_socket_client() {
  close_socket();
}

/*!
 * @brief construct and open client socket
 * @param bdb_socket_server unix domain server socket
 * @param errors if open errors
 * @details invariant: opened = socket is open
 */
Bdb_inet_socket_server::Bdb_inet_socket_client::Bdb_inet_socket_client(
    Bdb_inet_socket_server &bdb_socket_server,
    Bdb_errors &errors) {
  open_socket(bdb_socket_server, errors);
  if (!errors.has())
    opened = true;
}

/*!
 * @brief ensure client socket is closed
 * @details invariant: opened = socket is open
 */
void Bdb_inet_socket_server::Bdb_inet_socket_client::close_socket() {
  // Close the server socket
  if (opened) {
    close(client_socket);
    opened = false;
    std::cout << "Bdb_inet_socket_server::Bdb_socket_client::close_socket socket closed ("
              << client_socket << ")"
              << std::endl;
  }
}

/*!
 * @brief open socket client
 * @param errors if open errors
 * @details invariant: opened = socket is open
 */
void Bdb_inet_socket_server::Bdb_inet_socket_client::open_socket(
    Bdb_inet_socket_server &bdb_socket_server,
    Bdb_errors &errors) {
  // only called from constructor (once)
  opened = false;
  if (!bdb_socket_server.is_open())
    errors.add("Bdb_inet_socket_server::Bdb_socket_client::open_socket", "1",
               "Server socket not open");
  else {
    // Accept a client connection
    socklen_t client_address_length;
    memset(&client_inet_sockaddr, 0, sizeof(struct sockaddr_in));
    client_address_length = sizeof(client_inet_sockaddr);
    client_socket = accept(bdb_socket_server.get_server_socket(),
                           reinterpret_cast<sockaddr *>(&client_inet_sockaddr),
                           &client_address_length);
    opened = true;
    std::cout
        << "Bdb_inet_socket_server::Bdb_socket_client::open_socket client socket opened ("
        << client_socket << ")"
        << std::endl;
    if (client_socket == -1)
      errors.add("Bdb_inet_socket_server::Bdb_socket_client::open_socket", "2",
                 "Failed to accept client connection",
                 errno);
    else {
      if (getpeername(client_socket, (struct sockaddr *) &client_inet_sockaddr,
                      &client_address_length) == -1)
        errors.add("Bdb_inet_socket_server::Bdb_socket_client::open_socket", "3",
                   "Failed to get socket peername", errno);
      else {
        //char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_inet_sockaddr.sin_addr), client_ip, INET_ADDRSTRLEN);
        std::cout
            << "Bdb_inet_socket_server::Bdb_socket_client::open_socket: client socket ip address is '"
            << client_ip << "'" << std::endl;
      }
    }
  }
}

int Bdb_inet_socket_server::Bdb_inet_socket_client::read_socket(char *buffer, size_t buffer_len,
                                                                Bdb_errors &errors) const {
  if (!opened) {
    errors.add("Bdb_inet_socket_server::Bdb_socket_client::read_socket", "1",
               "Client socket not open");
    return 0;
  }
  // Read data from the client
  std::memset(buffer, 0, buffer_len);
  size_t bytes_read = recv(client_socket, buffer, buffer_len, 0);
  if (bytes_read == -1) {
    errors.add("Bdb_inet_socket_server::Bdb_socket_client::read_socket", "2",
               "Failed to read from client socket (" +
                   std::to_string(client_socket) + ")", errno);
    return 0;
  }
  if (bytes_read == 0) {
    // Client closed the connection
    return 0;
  }
  std::cout << "Bdb_inet_socket_server::Bdb_socket_client::read_socket: read "
            << std::endl << buffer << std::endl;
  return (int) bytes_read;
}

void
Bdb_inet_socket_server::Bdb_inet_socket_client::write_socket(const char *buffer, size_t buffer_len,
                                                             Bdb_errors &errors) const {
  if (!opened)
    errors.add("Bdb_inet_socket_server::Bdb_socket_client::write_socket", "1",
               "Client socket not open");
  if (!errors.has()) {
    std::cout << "Bdb_inet_socket_server::Bdb_socket_client::write_socket: writing "
              << std::endl << buffer << std::endl;
    size_t bytes_sent = send(client_socket, buffer, buffer_len, 0);
    if (bytes_sent == -1)
      errors.add("Bdb_inet_socket_server::Bdb_socket_client::write_socket", "2",
                 "Failed to send response to client",
                 errno);
    else if (bytes_sent != buffer_len)
      errors.add("Bdb_inet_socket_server::Bdb_socket_client::write_socket", "3",
                 "bytes written (" + std::to_string(bytes_sent) +
                     " not equal to number requested ("
                     + std::to_string(buffer_len) + ")");
  }
}
