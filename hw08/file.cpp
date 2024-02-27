#include "file.h"

#include "filesystem.h"

size_t File::get_size() const { return this->content.get_size(); }

bool File::rename(std::string_view new_name) {
  // TODO: Check that a filesystem actually exists, then rename it in the filesystem
  auto fs = filesystem.lock();
  if (fs) {
      if (fs->rename_file(name, new_name)) {
          this->name = std::string(new_name);
          return true;
      }
  }
  return false;
}

const std::string &File::get_name() const { return this->name; }

const FileContent &File::get_content() const { return this->content; }

File::File(FileContent&& content, std::weak_ptr<Filesystem> filesystem, std::string_view name)
  : content(std::move(content)), filesystem{std::move(filesystem)}, name(name) {}