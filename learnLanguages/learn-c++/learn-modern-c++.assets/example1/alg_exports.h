#ifndef alg_EXPORTS_H
#define alg_EXPORTS_H

#include <stddef.h>
#include <stdint.h>

#ifdef _WIN32
#define ALGO_BASE_API __declspec(dllexport)
#else
#define ALGO_BASE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define ALGO_BASE_ABI_VERSION 2u
#define ALGO_INSTANCE_ID_CAPACITY 128u
#define ALGO_TEXT_CAPACITY 128u

#define ALGO_INPUT_COUNT 3u
#define ALGO_TUNE_COUNT 2u
#define ALGO_IN_POINT_COUNT (ALGO_INPUT_COUNT + ALGO_TUNE_COUNT)
#define ALGO_OUTPUT_COUNT 2u

#define ALGO_INPUT_SIGNAL_A_INDEX 0u
#define ALGO_INPUT_SIGNAL_B_INDEX 1u
#define ALGO_INPUT_ENABLE_INDEX 2u
#define ALGO_TUNE_GAIN_INDEX 3u
#define ALGO_TUNE_BIAS_INDEX 4u

#define ALGO_OUTPUT_RESULT_INDEX 0u
#define ALGO_OUTPUT_READY_INDEX 1u

#define ALGO_STATUS_OK 0
#define ALGO_STATUS_RESULT_UNTRUSTWORTHY 1
#define ALGO_STATUS_ERROR (-1)

#define ALGO_ERROR_NONE 0
#define ALGO_ERROR_ABI_MISMATCH 1
#define ALGO_ERROR_STRUCT_SIZE_INVALID 2
#define ALGO_ERROR_INSTANCE_ID_INVALID 3
#define ALGO_ERROR_INSTANCE_NOT_FOUND 4
#define ALGO_ERROR_ALGORITHM_FAILED 5
#define ALGO_ERROR_RESULT_UNTRUSTWORTHY 6
#define ALGO_ERROR_POINTER_INVALID 7

#define ALGO_RC_OK 0
#define ALGO_RC_POINTER_INVALID (-1)
#define ALGO_RC_ABI_MISMATCH (-2)
#define ALGO_RC_STRUCT_SIZE_INVALID (-3)
#define ALGO_RC_INSTANCE_ID_INVALID (-4)
#define ALGO_RC_INSTANCE_NOT_FOUND (-5)
#define ALGO_RC_ALGORITHM_FAILED (-6)

// 首版模板算法约定:
// - input[0].ap.f64: signal_a
// - input[1].ap.f64: signal_b
// - input[2].dp: enable
// - input[3].ap.f64: gain (tune)
// - input[4].ap.f64: bias (tune)
// - output[0].ap.f64: result = (signal_a + signal_b) * gain + bias
// - output[1].dp: ready flag
typedef struct AlgoPoint {
    uint8_t point_status;
    uint8_t dp;
    uint8_t reserved[6];
    union {
        int32_t i32;
        uint32_t ui32;
        int16_t i16;
        uint16_t ui16;
        float f32;
        double f64;
        uint8_t bytes[8];
    } ap;
} AlgoPoint;

#if defined(__cplusplus)
#define ALGO_STATIC_ASSERT(condition, message) static_assert((condition), message)
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define ALGO_STATIC_ASSERT(condition, message) _Static_assert((condition), message)
#else
#define ALGO_STATIC_ASSERT(condition, message)
#endif

ALGO_STATIC_ASSERT(sizeof(AlgoPoint) == 16u, "AlgoPoint ABI changed: expected size 16");
ALGO_STATIC_ASSERT(offsetof(AlgoPoint, ap) == 8u, "AlgoPoint ABI changed: expected ap offset 8");

typedef struct AlgoIn {
    uint32_t abi_version;
    uint32_t struct_size;
    uint32_t period_ms;
    char instance_id[ALGO_INSTANCE_ID_CAPACITY];
    AlgoPoint points[ALGO_IN_POINT_COUNT];
} AlgoIn;

typedef struct AlgoOut {
    uint32_t abi_version;
    uint32_t struct_size;
    int32_t status;
    int32_t error_code;
    char text[ALGO_TEXT_CAPACITY];
    AlgoPoint points[ALGO_OUTPUT_COUNT];
} AlgoOut;

ALGO_BASE_API int alg_run(const AlgoIn* in, AlgoOut* out);
ALGO_BASE_API int alg_destroy(const char* instance_id);

#ifdef __cplusplus
}
#endif

#undef ALGO_STATIC_ASSERT

#endif  // alg_EXPORTS_H
