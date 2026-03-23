#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "alg_base.h"

#include <string>

class Algorithm final : public AlgBase {
public:
    explicit Algorithm(std::string instance_id);
    ~Algorithm() override = default;

private:
    struct TuneConfig {
        double gain;
        double bias;
    };

    int DoInit(const AlgoIn& in, AlgoOut& out) override;
    int DoReinit(const AlgoIn& in, AlgoOut& out) override;
    int DoRun(const AlgoIn& in, AlgoOut& out) override;
    bool NeedsReinitLocked(const AlgoIn& in) const override;
    void DoCleanupLocked() override;

    static TuneConfig ReadTuneConfig(const AlgoIn& in);
    static bool is_finite(double value);
    static bool almost_equal(double lhs, double rhs);

    TuneConfig tune_;
};

#endif  // ALGORITHM_H
