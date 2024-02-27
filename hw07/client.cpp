#include "client.h"
#include "socket.h"

namespace net {

Client::Client() : socket_{} {
    Socket socket {};
    socket_ = std::move(socket);
};

Connection Client::connect(uint16_t port) {
    socket_.connect(port);
    return socket_.accept();
}

Connection Client::connect(std::string destination, uint16_t port) {
    socket_.connect(destination, port);
    return socket_.accept();
}

} // namespace net
