#include "alg_base.h"

#include <cstring>

namespace {

void copy_text(char* destination, const char* source) {
    if (destination == nullptr) {
        return;
    }

    if (source == nullptr) {
        destination[0] = '\0';
        return;
    }

    std::strncpy(destination, source, ALGO_TEXT_CAPACITY - 1u);
    destination[ALGO_TEXT_CAPACITY - 1u] = '\0';
}

}  // namespace

AlgBase::AlgBase(std::string instance_id)
    : instance_id_(std::move(instance_id)), initialized_(false) {}

int AlgBase::Execute(const AlgoIn& in, AlgoOut& out, bool is_new_instance) {
    std::lock_guard<std::mutex> lock(mutex_);

    ResetOutput(out);

    int rc = ALGO_RC_OK;
    if (is_new_instance || !initialized_) {
        rc = DoInit(in, out);
        if (rc == ALGO_RC_OK) {
            initialized_ = true;
        }
    } else if (NeedsReinitLocked(in)) {
        rc = DoReinit(in, out);
    }

    if (rc != ALGO_RC_OK) {
        if (out.error_code == ALGO_ERROR_NONE) {
            SetError(out, ALGO_ERROR_ALGORITHM_FAILED, "algorithm initialization failed");
        }
        return rc;
    }

    rc = DoRun(in, out);
    if (rc != ALGO_RC_OK && out.error_code == ALGO_ERROR_NONE) {
        SetError(out, ALGO_ERROR_ALGORITHM_FAILED, "algorithm run failed");
    }
    return rc;
}

void AlgBase::Cleanup() {
    std::lock_guard<std::mutex> lock(mutex_);
    DoCleanupLocked();
    initialized_ = false;
}

void AlgBase::DoCleanupLocked() {}

void AlgBase::SetOk(AlgoOut& out) const {
    out.status = ALGO_STATUS_OK;
    out.error_code = ALGO_ERROR_NONE;
    out.text[0] = '\0';
}

void AlgBase::SetError(AlgoOut& out, int32_t error_code, const char* text) const {
    out.status = ALGO_STATUS_ERROR;
    out.error_code = error_code;
    copy_text(out.text, text);
}

void AlgBase::SetUntrustworthy(AlgoOut& out, const char* text) const {
    out.status = ALGO_STATUS_RESULT_UNTRUSTWORTHY;
    out.error_code = ALGO_ERROR_RESULT_UNTRUSTWORTHY;
    copy_text(out.text, text);
}

void AlgBase::ResetOutput(AlgoOut& out) {
    out.status = ALGO_STATUS_ERROR;
    out.error_code = ALGO_ERROR_NONE;
    out.text[0] = '\0';
    std::memset(out.points, 0, sizeof(out.points));
}

double AlgBase::GetInputAp(const AlgoIn& in, std::size_t input_var_pos) const {
    return in.points[input_var_pos].ap.f64;
}

bool AlgBase::IsInputDpSet(const AlgoIn& in, std::size_t input_var_pos) const {
    return in.points[input_var_pos].dp != 0;
}
