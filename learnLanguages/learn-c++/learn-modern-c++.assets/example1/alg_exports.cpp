#include "alg_exports.h"

#include "alg_manager.h"

#include <cstring>
#include <string>

namespace {

RcnyAlgManager& manager() {
    static RcnyAlgManager instance;
    return instance;
}

void reset_output_payload(AlgoOut* out) {
    if (out == nullptr) {
        return;
    }
    out->status = ALGO_STATUS_ERROR;
    out->error_code = ALGO_ERROR_NONE;
    out->text[0] = '\0';
    std::memset(out->points, 0, sizeof(out->points));
}

void write_text(char* destination, const char* text) {
    if (destination == nullptr) {
        return;
    }
    if (text == nullptr) {
        destination[0] = '\0';
        return;
    }
    std::strncpy(destination, text, ALGO_TEXT_CAPACITY - 1u);
    destination[ALGO_TEXT_CAPACITY - 1u] = '\0';
}

void set_output_error(AlgoOut* out, int32_t error_code, const char* text) {
    if (out == nullptr) {
        return;
    }
    reset_output_payload(out);
    out->status = ALGO_STATUS_ERROR;
    out->error_code = error_code;
    write_text(out->text, text);
}

bool is_valid_output_abi(const AlgoOut* out) {
    return out != nullptr && out->abi_version == ALGO_BASE_ABI_VERSION &&
           out->struct_size >= sizeof(AlgoOut);
}

bool make_instance_id(const char* raw, size_t capacity, std::string* result) {
    if (raw == nullptr || result == nullptr) {
        return false;
    }

    const void* terminator = std::memchr(raw, '\0', capacity);
    if (terminator == nullptr) {
        return false;
    }

    const auto length = static_cast<size_t>(static_cast<const char*>(terminator) - raw);
    if (length == 0u) {
        return false;
    }

    result->assign(raw, length);
    return true;
}

int validate_args(const AlgoIn* in, AlgoOut* out) {
    if (in == nullptr || out == nullptr) {
        return ALGO_RC_POINTER_INVALID;
    }

    if (in->struct_size < sizeof(AlgoIn) || out->struct_size < sizeof(AlgoOut)) {
        return ALGO_RC_STRUCT_SIZE_INVALID;
    }

    if (in->abi_version != ALGO_BASE_ABI_VERSION || out->abi_version != ALGO_BASE_ABI_VERSION) {
        return ALGO_RC_ABI_MISMATCH;
    }

    return ALGO_RC_OK;
}

}  // namespace

extern "C" int alg_run(const AlgoIn* in, AlgoOut* out) {
    const int validation_rc = validate_args(in, out);
    if (validation_rc != ALGO_RC_OK) {
        if (validation_rc == ALGO_RC_POINTER_INVALID) {
            return validation_rc;
        }

        if (validation_rc == ALGO_RC_STRUCT_SIZE_INVALID) {
            if (is_valid_output_abi(out)) {
                set_output_error(out, ALGO_ERROR_STRUCT_SIZE_INVALID, "invalid structure size");
            }
            return validation_rc;
        }

        if (is_valid_output_abi(out)) {
            set_output_error(out, ALGO_ERROR_ABI_MISMATCH, "ABI mismatch");
        }
        return validation_rc;
    }

    reset_output_payload(out);

    std::string instance_id;
    if (!make_instance_id(in->instance_id, ALGO_INSTANCE_ID_CAPACITY, &instance_id)) {
        set_output_error(out, ALGO_ERROR_INSTANCE_ID_INVALID, "invalid instance id");
        return ALGO_RC_INSTANCE_ID_INVALID;
    }

    auto [instance, created] = manager().GetOrCreate(instance_id);
    const int rc = instance->Execute(*in, *out, created);
    if (rc != ALGO_RC_OK && out->error_code == ALGO_ERROR_NONE) {
        set_output_error(out, ALGO_ERROR_ALGORITHM_FAILED, "algorithm failed");
    }

    return rc;
}

extern "C" int alg_destroy(const char* instance_id) {
    std::string key;
    if (!make_instance_id(instance_id, ALGO_INSTANCE_ID_CAPACITY, &key)) {
        return ALGO_RC_INSTANCE_ID_INVALID;
    }
    return manager().Destroy(key);
}
