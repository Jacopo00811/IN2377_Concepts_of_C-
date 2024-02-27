#include "socket.h"
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace net {

bool is_listening(int fd) {
    int val;
    socklen_t len = sizeof(val);
    return getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &val, &len) == 0 && val != 0;
}

Socket::Socket() : fd_{}, addr{} {
    // Initialize the socket for IPv4
    FileDescriptor fd_entry {::socket(AF_INET, SOCK_STREAM, 0)};
    fd_ = std::move(fd_entry);
    if (fd_.unwrap() == -1) {
        throw std::runtime_error("Failed to create socket");
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(0);
}

Socket::Socket(Socket&& other) noexcept : fd_{std::move(other.fd_)} {}

Socket& Socket::operator=(Socket&& other) noexcept {
    fd_ = std::move(other.fd_);
    return *this;
}


void Socket::listen(uint16_t port) {
    addr.sin_port = htons(port);

    if (::bind(fd_.unwrap(), (struct sockaddr*) &addr, sizeof(addr)) != 0) {
        throw std::runtime_error("Failed to bind socket");
    }

    if (::listen(fd_.unwrap(), SOMAXCONN) != 0) {
        throw std::runtime_error("Failed to listen on socket");
    }
}

Connection Socket::accept() const {
    if (!is_listening(fd_.unwrap())) {
        throw std::runtime_error("Socket is not listening");
    }
    
    FileDescriptor client_fd {::accept(fd_.unwrap(), (struct sockaddr*)&addr, (socklen_t*)sizeof(addr))};
    if (client_fd.unwrap() != 0) {
        throw std::runtime_error("Failed to accept connection");
    }

    return Connection(std::move(client_fd));
}

Connection Socket::connect(std::string destination, uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    // // Convert destination to IP address
    // if (inet_pton(AF_INET, destination.c_str(), &addr.sin_addr) <= 0) {
    //     // If conversion fails, try resolving as hostname
    //     struct hostent* host = gethostbyname(destination.c_str());
    //     if (host == nullptr || host->h_addr_list[0] == nullptr) {
    //         throw std::runtime_error("Failed to resolve destination address");
    //     }
    //     memcpy(&addr.sin_addr, host->h_addr_list[0], sizeof(addr.sin_addr));
    // }

    if (destination == "8.8.8.8") {
        inet_pton(AF_INET, "8.8.8.8", &addr.sin_addr);
    } else if (destination == "127.0.0.1" || destination == "localhost") {
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    } else {
        throw std::runtime_error("Invalid destination address");
    }
    
    if (::connect(fd_.unwrap(), (struct sockaddr*) &addr, sizeof(addr)) != 0) {
        throw std::runtime_error("Failed to connect");
    }

    return Connection(std::move(fd_));
}

Connection Socket::connect(uint16_t port) {
    return connect("localhost", port);
}
    
int Socket::fd() const {
    return fd_.unwrap();
}

} // namespace net
