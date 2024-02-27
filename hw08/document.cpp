#include "document.h"
#include <algorithm>

Document::Document(FileContent &&content) : File{std::move(content)} {}

// TODO implement get_type function
std::string_view Document::get_type() const {
  return "DOC";
}

size_t Document::get_raw_size() const {
  return this->get_size();
}

unsigned Document::get_character_count() const {
  std::shared_ptr<const std::string> content_ptr = {content.get()};
  std::string content_string = *content_ptr;
  
  return static_cast<unsigned int>(std::count_if(content_string.begin(), content_string.end(), [](char c) { return !std::isspace(c); }));
}

// TODO implement content update function
void Document::update(FileContent&& new_content) {
  content = std::move(new_content);
}