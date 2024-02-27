#include "filesystem.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <iostream>

Filesystem::Filesystem() {}

bool Filesystem::register_file(const std::string &name, std::shared_ptr<File> file) {
  // TODO: Do sanity checks first! Is that actually a good name, is the pointer set,
  // does a file with that name already exists? Anything else to check here?
  if (name.empty() || file == nullptr || files.find(name) != files.end())
    return false;   

  // TODO: Check if the file already has a filesystem, if so, it may only be
  // the same as this one, not another one!
 
  if (file->filesystem.lock()) {
    return false;  // File already belongs to a filesystem
  }

  file->name = name;
  // Create a shared pointer to the this object
  auto thisptr = this->shared_from_this();

  // Set the filesystem link in the file
  file->filesystem = std::move(thisptr);

  // Store the file in the filesystem
  // TODO: Now actually store it in the filesystem
  files[name] = std::move(file);
  return true;
}

bool Filesystem::remove_file(std::string_view name) {
  // TODO: Remove file from the filesystem, return true if successful, false otherwhise.
  // false may indicate multple different reasons for failure, which is not impotant
  auto it = files.find(std::string(name));
  if (it != files.end()) {
    files.erase(it);
    return true;
  }
  return false;
}

bool Filesystem::rename_file(std::string_view source, std::string_view dest) {
  // TODO: Check filesystem for the source and dest files, if it's possible to rename
  // the source file, then update the filesystem
  if (source.empty() || dest.empty() || source == dest) {
    return false;
  }

  auto sourceIter = files.find(std::string(source));
  auto destIter = files.find(std::string(dest));

  if (sourceIter != files.end() && destIter == files.end()) {
    // Source file exists, destination file does not exist
    auto file = sourceIter->second;
    files.erase(sourceIter);
    files[std::string(dest)] = file;
    file->name = std::string(dest);
    return true;
  }

  // Source file doesn't exist or destination file already exists
  return false;
}

std::shared_ptr<File> Filesystem::get_file(std::string_view name) const {
  // TODO: Check the filesystem for the given file, return it if you found it,
  //       else just return nothing
  auto it = files.find(static_cast<std::string>(name));
  if (it != files.end())
    return it->second;

  return nullptr;  // File not found
}

size_t Filesystem::get_file_count() const {
  // TODO: how many files have you got?
  return files.size();
}

size_t Filesystem::in_use() const {
  // TODO: sum up all real file sizes
  return std::accumulate(files.begin(), files.end(), 0,
                         [](size_t sum, const auto &file) {
                           return sum + file.second->get_size();
                         });
}

std::vector<std::shared_ptr<File>> Filesystem::files_in_size_range(size_t max, size_t min) const {
  // TODO: find the matching files and return them
  std::vector<std::shared_ptr<File>> result;

  for (const auto &file : files) {
    size_t fileSize = file.second->get_size();
    if (fileSize >= min && fileSize <= max) {
      result.push_back(file.second);
    }
  }

  return result;
}

// convenience function so you can see what files are stored
std::string Filesystem::file_overview(bool sort_by_size) {
  std::ostringstream output;
  // this function is not tested, but it may help you when debugging.

  output << "files in filesystem: " << std::endl;

  // for (auto&& entry : this->files) {
  //     // TODO: fix printing name, type and size
  //     output << entry.get_name << std::endl;
  // }
  return std::move(output).str();
}
