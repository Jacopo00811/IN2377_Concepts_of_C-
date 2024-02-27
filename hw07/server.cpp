#include "server.h"
#include "socket.h"
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>

namespace net {

Server::Server(uint16_t port) {
    Socket socket {};
    socket_ = std::move(socket);
    socket_.listen(port);
}

Connection Server::accept() const {
   socket_.accept();
   return socket_.accept();
}

} // namespace net
