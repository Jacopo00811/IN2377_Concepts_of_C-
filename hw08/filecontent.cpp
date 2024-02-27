#include "filecontent.h"

FileContent::FileContent(const std::string& content) : content_(std::make_shared<std::string>(content)) {}

FileContent::FileContent(std::string&& content) : content_(std::make_shared<std::string>(std::move(content))) {}

FileContent::FileContent(const char* content) : content_(std::make_shared<std::string>(content)) {}

size_t FileContent::get_size() const {
    return content_->size();
}

std::shared_ptr<const std::string> FileContent::get() const {
    return content_;
}
