// connection.cpp
#include "connection.h"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h> 

namespace net {

ssize_t send(int fd, std::span<const char> data) {
    return ::send(fd, data.data(), data.size(), 0);
}

ssize_t receive(int fd, std::span<char> buf) {
    return ::recv(fd, buf.data(), buf.size(), 0);
}

Connection::Connection(FileDescriptor&& fd) : fd_(std::move(fd)) {}

void Connection::send(std::string_view data) const {
    ssize_t sent = ::send(fd_.unwrap(), data.data(), data.size(), 0);
    if (sent == -1) {
        // Handle error, you might want to throw an exception or log an error message
        std::cerr << "Error sending data: " << strerror(errno) << std::endl;
    }
}

void Connection::send(std::istream& data) const {
    constexpr size_t buffer_size = 128;
    char buffer[buffer_size];
    
    while (data.good()) {
        data.read(buffer, buffer_size);
        ssize_t sent = ::send(fd_.unwrap(), buffer, data.gcount(), 0);
        if (sent == -1) {
            // Handle error, you might want to throw an exception or log an error message
            std::cerr << "Error sending data: " << strerror(errno) << std::endl;
            break;
        }
    }
}

ssize_t Connection::receive(std::ostream& stream) const {
    constexpr size_t buffer_size = 128;
    char buffer[buffer_size];
    ssize_t received = ::recv(fd_.unwrap(), buffer, buffer_size, 0);

    if (received > 0) {
        // Write the received data to the stream
        stream.write(buffer, received);
    }

    return received;
}

ssize_t Connection::receive_all(std::ostream& stream) const {
    constexpr size_t buffer_size = 128;
    char buffer[buffer_size];
    ssize_t total_received = 0;

    while (true) {
        size_t received = ::recv(fd_.unwrap(), buffer, buffer_size, MSG_WAITALL); // 0
        if (received <= 0) {
            break;  // No more data or an error occurred
        }

        // Write the received data to the stream
        stream.write(buffer, received);
        total_received += received;
    }

    return total_received;
}

int Connection::fd() const {
    return fd_.unwrap();
}

} // namespace net