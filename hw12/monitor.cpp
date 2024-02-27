#include "monitor.h"

#include <thread>
#include <iostream>
#include <stack>


FileMonitor::FileMonitor(const std::string &targetpath,
                         std::chrono::milliseconds interval,
                         const std::string &logfilename)
    : logger{logfilename}
    , interval{interval}
    , targetPath_{targetpath}
{
    // TODO: Initialize a mapping for all files in the target path
    std::stack<std::filesystem::path> directories;
    directories.push(targetpath);
    while (!directories.empty()) {
        auto currentPath = directories.top();
        directories.pop();

        for (const auto &entry : std::filesystem::directory_iterator(currentPath)) {
            this->fileLastModified.emplace(entry.path().string(), std::filesystem::last_write_time(entry.path()));
            if (std::filesystem::is_directory(entry.path())) {
                directories.push(entry);
            }
        }
    }

    // for (const auto &entry : std::filesystem::recursive_directory_iterator(targetpath)) {
    //     //auto path = std::filesystem::relative(entry.path(), std::filesystem::current_path()).string();
    //     //fileLastModified[path] = std::filesystem::last_write_time(entry.path());
        
    //     fileLastModified[entry.path().string()] = std::filesystem::last_write_time(entry.path());

    //     // auto absolutePath = std::filesystem::absolute(entry.path()).string();
    //     // fileLastModified[absolutePath] = std::filesystem::last_write_time(entry.path());
    // }
}

// Monitor the targetpath for changes and pass information to the logger in case of differences
void FileMonitor::start(std::chrono::seconds timeout) {
    // TODO: Monitor the system for the given amount of time.
    auto startTime = std::chrono::system_clock::now();
    do {
        for (auto it = fileLastModified.begin(); it != fileLastModified.end();) {
            const auto &file = *it;
            if (!std::filesystem::exists(file.first)) {
                // File is deleted
                logger.log(file.first, status::removed);
                it = fileLastModified.erase(it);
            } else {
                ++it;
            }
        }

        std::stack<std::filesystem::path> directories;
        directories.push(targetPath_);
        while (!directories.empty()) {
            auto direction = directories.top();
            directories.pop();

            for (const auto& entry : std::filesystem::directory_iterator(direction)) {
                if (fileLastModified.count(entry.path().string()) == 0) {
                    fileLastModified.emplace(entry.path().string(), std::filesystem::last_write_time(entry.path()));
                    logger.log(entry.path().string(), status::added);
                }


                auto currentWriteTime = std::filesystem::last_write_time(entry.path());
                if (currentWriteTime != fileLastModified[entry.path().string()]) {
                    fileLastModified[entry.path().string()] = std::filesystem::last_write_time(entry.path());
                    logger.log(entry.path().string(), status::changed);
                }

                if (entry.is_directory()) {
                    directories.push(entry);
                }
            }   

        }

        std::this_thread::sleep_for(interval);
    } while (std::chrono::system_clock::now() - startTime < timeout);
        

    // // Every given interval, check for modifications to the files and log it
    // auto startTime = std::chrono::system_clock::now();
    // auto endTime = startTime + timeout;

    // while (std::chrono::system_clock::now() < endTime) {
    //     // Check for modifications every interval
    //     std::this_thread::sleep_for(interval);

    //     for (const auto &entry : std::filesystem::recursive_directory_iterator(std::filesystem::current_path())) {
    //         auto path = entry.path().string();
            
    //         //auto path = std::filesystem::relative(entry.path(), std::filesystem::current_path()).string();
            
    //         //auto absolutePath = std::filesystem::absolute(entry.path()).string();

    //         auto lastModified = std::filesystem::last_write_time(entry.path());

    //         if (fileLastModified.find(path) == fileLastModified.end()) {
    //             // File is added
    //             fileLastModified.emplace(path, lastModified);
    //             logger.log(path, status::added);
    //         } else if (fileLastModified[path] != lastModified) {
    //             // File is modified
    //             fileLastModified[path] = lastModified;
    //             logger.log(path, status::changed);
    //         }
    //     }

    //     // Check for deleted files
    //     for (auto it = fileLastModified.begin(); it != fileLastModified.end();) {
    //         if (!std::filesystem::exists(it->first)) {
    //             // File is deleted
    //             logger.log(it->first, status::removed);
    //             it = fileLastModified.erase(it);
    //         } else {
    //             ++it;
    //         }
    //     }
    // }
}