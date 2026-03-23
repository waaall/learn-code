#ifndef ALG_BASE_H
#define ALG_BASE_H

#include "alg_exports.h"

#include <cstddef>
#include <mutex>
#include <string>

class AlgBase {
public:
    explicit AlgBase(std::string instance_id);
    virtual ~AlgBase() = default;

    int Execute(const AlgoIn& in, AlgoOut& out, bool is_new_instance);
    void Cleanup();

protected:
    virtual int DoInit(const AlgoIn& in, AlgoOut& out) = 0;
    virtual int DoReinit(const AlgoIn& in, AlgoOut& out) = 0;
    virtual int DoRun(const AlgoIn& in, AlgoOut& out) = 0;
    virtual bool NeedsReinitLocked(const AlgoIn& in) const = 0;
    virtual void DoCleanupLocked();

    void SetOk(AlgoOut& out) const;
    void SetError(AlgoOut& out, int32_t error_code, const char* text) const;
    void SetUntrustworthy(AlgoOut& out, const char* text) const;
    static void ResetOutput(AlgoOut& out);
    double GetInputAp(const AlgoIn& in, std::size_t input_var_pos) const;
    bool IsInputDpSet(const AlgoIn& in, std::size_t input_var_pos) const;

private:
    std::string instance_id_;
    std::mutex mutex_;
    bool initialized_;
};

#endif  // ALG_BASE_H
