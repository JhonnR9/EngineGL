//
// Created by jhone on 04/04/2026.
//

#include "assets_manager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>

AssetsManager::AssetsManager() = default;

AssetsManager::~AssetsManager() {
    pool_works();
}

void AssetsManager::load_file(const char* path) {
    std::filesystem::path p(path);
    std::ifstream file(p, std::ios::binary | std::ios::ate);

    if (!file) {
        std::cerr << "Error opening file: " << path << "\n";
        return;
    }

    std::streamsize size = file.tellg();
    if (size <= 0) return;
    file.seekg(0, std::ios::beg);

    std::vector<char> raw_bytes(size);

    if (!file.read(raw_bytes.data(), size)) {
        std::cerr << "Error reading file: " << path << "\n";
        return;
    }

    std::cout << "File loaded successfully! Size: " << raw_bytes.size() << " bytes\n";

    {
        std::lock_guard<std::mutex> lock(mtx);
        files_preload[path] = std::move(raw_bytes);
    }
}

bool AssetsManager::is_file_preload(const char* path) {
    std::lock_guard<std::mutex> lock(mtx);
    return files_preload.contains(path);
}

void AssetsManager::queue_file_load(const char* path) {
    std::string path_str(path);

    std::lock_guard<std::mutex> lock(mtx);

    if (works.contains(path_str)) {
        return;
    }

    std::thread t([this, path_str]() {
        this->load_file(path_str.c_str());
    });

    works[path_str] = std::move(t);
    dirty = true;
}

void AssetsManager::pool_works() {
    if (!dirty) return;

    std::map<std::string, std::thread> threads_to_join;
    {
        std::lock_guard<std::mutex> lock(mtx);
        threads_to_join = std::move(works);
        dirty = false;
    }

    for (auto& [path, t] : threads_to_join) {
        if (t.joinable()) t.join();
    }
}
