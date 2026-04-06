#ifndef ASSETS_MANAGER_H
#define ASSETS_MANAGER_H

#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <vector>
#include <string>

#include "asset.h"

class AssetsManager {
    std::map<std::string, std::thread> works;
    std::map<std::string, std::vector<char> > files_preload;
    std::map<std::string, std::shared_ptr<Asset>> assets_cache;

    std::mutex mtx;
    bool dirty{false};

public:
    AssetsManager();

    ~AssetsManager();

    void queue_file_load(const char *path);

    void load_file(const char *path);

    bool is_file_preload(const char *path);

    void pool_works();

    template<typename T, typename... Args>
    std::shared_ptr<T> get_asset(const std::string &path, Args &&... args);
};

template<typename T, typename... Args>
std::shared_ptr<T> AssetsManager::get_asset(const std::string &path, Args &&... args) {

    std::unique_lock<std::mutex> lock(mtx);


    if (assets_cache.contains(path)) {
        auto casted = std::dynamic_pointer_cast<T>(assets_cache[path]);
        if (!casted) {
            throw std::runtime_error("[AssetsManager] Asset type mismatch for: " + path);
        }
        return casted;
    }


    if (!files_preload.contains(path)) {
        if (works.contains(path)) {
            return nullptr;
        }

        std::cerr << "[AssetsManager] Not queued: " << path << "\n";
        return nullptr;
    }


    std::vector<char> bytes = std::move(files_preload[path]);
    files_preload.erase(path);


    lock.unlock();


    auto asset = std::make_shared<T>(
        bytes,
        std::forward<Args>(args)...
    );


    lock.lock();

    if (assets_cache.contains(path)) {
        auto casted = std::dynamic_pointer_cast<T>(assets_cache[path]);
        if (!casted) {
            throw std::runtime_error("[AssetsManager] Asset type mismatch for: " + path);
        }
        return casted;
    }

    assets_cache[path] = asset;
    return asset;
}



#endif // ASSETS_MANAGER_H
