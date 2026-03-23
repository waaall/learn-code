#include "alg_manager.h"

#include "algorithm.h"

std::pair<std::shared_ptr<AlgBase>, bool> RcnyAlgManager::GetOrCreate(const std::string& instance_id) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = instances_.find(instance_id);
    if (it != instances_.end()) {
        return {it->second, false};
    }

    auto algorithm = std::make_shared<Algorithm>(instance_id);
    instances_.emplace(instance_id, algorithm);
    return {std::move(algorithm), true};
}

int RcnyAlgManager::Destroy(const std::string& instance_id) {
    std::shared_ptr<AlgBase> instance;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = instances_.find(instance_id);
        if (it == instances_.end()) {
            return ALGO_RC_INSTANCE_NOT_FOUND;
        }
        instance = it->second;
        instances_.erase(it);
    }

    instance->Cleanup();
    return ALGO_RC_OK;
}
