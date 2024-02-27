#include "logger.h"

#include <ctime>
#include <chrono>
#include <filesystem>
#include <iostream>



Logger::Logger(const std::string &filename) : file{} {

    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    
    // Backup old log file if it exists
    if (std::filesystem::exists(filename)) {
        if (std::filesystem::exists(filename + "_old")) {
            std::filesystem::remove(filename + "_old");
            std::filesystem::rename(filename, filename + "_old");

        } else {
            std::filesystem::rename(filename, filename + "_old");
        }
    }
    
    // Open the log file for writing
    file = std::ofstream(filename);

    file << "Commencing logging for directory: "
         << std::filesystem::current_path()
         << std::endl
         << "Current time is: "
         << std::ctime(&time)
         << std::endl;

}


void Logger::log(const std::string &path, status what) {
    if (not file.is_open()) {
        throw std::runtime_error("File could not be opened!");
    }

    switch (what) {
    case status::added :
        file << "+ File was added:    " << path << std::endl;
        break;
    case status::changed :
        file << "! File was modified: " << path << std::endl;
        break;
    case status::removed :
        file << "~ File was deleted:  " << path << std::endl;
        break;
    default:
        file << "? Error! Unknown file status detected for: " << path << std::endl;
        break;
    }
}