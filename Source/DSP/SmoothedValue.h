#pragma once

#include <algorithm>
#include <cmath>

namespace motionfield {

class SmoothedValue {
public:
    void prepare(double sampleRate, double rampSeconds = 0.025) noexcept {
        rampSamples_ = std::max(1, static_cast<int>(std::round(sampleRate * rampSeconds)));
    }

    void reset(double value) noexcept {
        current_ = target_ = value;
        step_ = 0.0;
        remaining_ = 0;
    }

    void setTarget(double value) noexcept {
        if (std::abs(value - target_) <= 1.0e-12) return;
        target_ = value;
        remaining_ = rampSamples_;
        step_ = (target_ - current_) / static_cast<double>(remaining_);
    }

    [[nodiscard]] double next() noexcept {
        if (remaining_ > 0) {
            current_ += step_;
            if (--remaining_ == 0) current_ = target_;
        }
        return current_;
    }

private:
    double current_{};
    double target_{};
    double step_{};
    int rampSamples_{1};
    int remaining_{};
};

} // namespace motionfield
