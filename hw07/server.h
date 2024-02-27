#pragma once

#include "socket.h"

namespace net {

/**
 * TCP socket server. Listens for your request to deliver you juicy data!
 *
 * TODOs:
 * - Server must be constructible from a port
 * - Server must have a function accept, which listens on the given port and returns a new Connection
 */
class Server {
public:
    /// Constructor that takes a port
    Server(uint16_t port);

    /// Accept new connections
    Connection accept() const;

private:
    Socket socket_;
};

} // namespace net

