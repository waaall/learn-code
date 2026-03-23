#ifndef alg_MANAGER_H
#define alg_MANAGER_H

#include "alg_base.h"

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>

class RcnyAlgManager {
public:
    std::pair<std::shared_ptr<AlgBase>, bool> GetOrCreate(const std::string& instance_id);
    int Destroy(const std::string& instance_id);

private:
    std::mutex mutex_;
    std::unordered_map<std::string, std::shared_ptr<AlgBase>> instances_;
};

#endif  // alg_MANAGER_H
