#include "filedescriptor.h"
#include <unistd.h>
// TODO Implement the methods of the FileDescriptor class as given in the filedescriptor.h header.

namespace net {

FileDescriptor::FileDescriptor() = default;

FileDescriptor::FileDescriptor(int fd) : fd_(fd) {}

FileDescriptor::~FileDescriptor() {
    if (fd_.has_value() && fd_ != -1) {
        close(fd_.value());
    }
}

// Implement copy and move constructors and assignment operators
// Copy constructor
//FileDescriptor::FileDescriptor(const FileDescriptor& other) : fd_(other.fd_) {}
//FileDescriptor::FileDescriptor(const FileDescriptor& other) = delete;

// Move constructor
FileDescriptor::FileDescriptor(FileDescriptor&& other) noexcept : fd_(std::exchange(other.fd_, -1)) {}

// Copy assignment
// FileDescriptor& FileDescriptor::operator=(const FileDescriptor& other) {
//     if (fd_.has_value()) {
//         close(fd_.value());
//     }

//     fd_ = other.fd_;
//     return *this;
// }
//FileDescriptor& FileDescriptor::operator=(const FileDescriptor& other) = delete;

// Move assignment
FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other) noexcept {
    if (fd_.has_value() && fd_.value() != -1) {
        close(fd_.value());
    }

    fd_ = std::exchange(other.fd_, -1);
    return *this;
}

int FileDescriptor::unwrap() const {
    // if (!fd_.has_value()) {
    //     return -1;
    // }

    // return fd_.value();
    return fd_.value_or(-1);
}

} // namespace net
