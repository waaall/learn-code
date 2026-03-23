#include "algorithm.h"

#include <cmath>

namespace {

constexpr double kTuneTolerance = 1e-9;

}  // namespace

Algorithm::Algorithm(std::string instance_id)
    : AlgBase(std::move(instance_id)), tune_{1.0, 0.0} {}

int Algorithm::DoInit(const AlgoIn& in, AlgoOut& out) {
    tune_ = ReadTuneConfig(in);
    if (!is_finite(tune_.gain) || !is_finite(tune_.bias)) {
        SetError(out, ALGO_ERROR_ALGORITHM_FAILED, "invalid tune configuration");
        return ALGO_RC_ALGORITHM_FAILED;
    }
    return ALGO_RC_OK;
}

int Algorithm::DoReinit(const AlgoIn& in, AlgoOut& out) {
    tune_ = ReadTuneConfig(in);
    if (!is_finite(tune_.gain) || !is_finite(tune_.bias)) {
        SetError(out, ALGO_ERROR_ALGORITHM_FAILED, "invalid tune configuration");
        return ALGO_RC_ALGORITHM_FAILED;
    }
    return ALGO_RC_OK;
}

int Algorithm::DoRun(const AlgoIn& in, AlgoOut& out) {
    const bool enabled = IsInputDpSet(in, ALGO_INPUT_ENABLE_INDEX);
    if (!enabled) {
        out.points[ALGO_OUTPUT_READY_INDEX].dp = 0;
        SetUntrustworthy(out, "algorithm result is untrustworthy");
        return ALGO_RC_OK;
    }

    const double signal_a = GetInputAp(in, ALGO_INPUT_SIGNAL_A_INDEX);
    const double signal_b = GetInputAp(in, ALGO_INPUT_SIGNAL_B_INDEX);

    if (!is_finite(signal_a) || !is_finite(signal_b) ||
        !is_finite(tune_.gain) || !is_finite(tune_.bias)) {
        out.points[ALGO_OUTPUT_READY_INDEX].dp = 0;
        SetUntrustworthy(out, "algorithm result is untrustworthy");
        return ALGO_RC_OK;
    }

    const double result = (signal_a + signal_b) * tune_.gain + tune_.bias;
    if (!is_finite(result)) {
        out.points[ALGO_OUTPUT_READY_INDEX].dp = 0;
        SetUntrustworthy(out, "algorithm result is untrustworthy");
        return ALGO_RC_OK;
    }

    out.points[ALGO_OUTPUT_RESULT_INDEX].ap.f64 = result;
    out.points[ALGO_OUTPUT_READY_INDEX].dp = 1;
    SetOk(out);
    return ALGO_RC_OK;
}

bool Algorithm::NeedsReinitLocked(const AlgoIn& in) const {
    const TuneConfig next = ReadTuneConfig(in);
    return !almost_equal(tune_.gain, next.gain) || !almost_equal(tune_.bias, next.bias);
}

void Algorithm::DoCleanupLocked() {
    tune_ = TuneConfig{1.0, 0.0};
}

Algorithm::TuneConfig Algorithm::ReadTuneConfig(const AlgoIn& in) {
    return TuneConfig{
        in.points[ALGO_TUNE_GAIN_INDEX].ap.f64,
        in.points[ALGO_TUNE_BIAS_INDEX].ap.f64,
    };
}

bool Algorithm::is_finite(double value) {
    return std::isfinite(value);
}

bool Algorithm::almost_equal(double lhs, double rhs) {
    return std::fabs(lhs - rhs) <= kTuneTolerance;
}
