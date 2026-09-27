#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace motionfield {

template <typename Sample>
class CubicDelayLine {
public:
    void prepare(std::size_t maximumDelaySamples) {
        buffer_.assign(maximumDelaySamples + 8U, Sample{});
        writeIndex_ = 0U;
    }

    void reset() noexcept {
        std::fill(buffer_.begin(), buffer_.end(), Sample{});
        writeIndex_ = 0U;
    }

    void push(Sample sample) noexcept {
        buffer_[writeIndex_] = sample;
        writeIndex_ = (writeIndex_ + 1U) % buffer_.size();
    }

    [[nodiscard]] Sample read(double requestedDelaySamples) const {
        if (buffer_.empty()) throw std::logic_error("delay line is not prepared");
        const auto maximum = static_cast<double>(buffer_.size() - 4U);
        const auto delay = std::clamp(requestedDelaySamples, 1.0, maximum);
        const auto whole = static_cast<long long>(std::floor(delay));
        const auto mu = delay - static_cast<double>(whole);
        const auto base = static_cast<long long>(writeIndex_) - whole;

        const auto y1 = static_cast<double>(buffer_[wrappedIndex(base)]);
        const auto y2 = static_cast<double>(buffer_[wrappedIndex(base - 1LL)]);
        if (delay < 2.0)
            return static_cast<Sample>((1.0 - mu) * y1 + mu * y2);

        const auto y0 = static_cast<double>(buffer_[wrappedIndex(base + 1LL)]);
        const auto y3 = static_cast<double>(buffer_[wrappedIndex(base - 2LL)]);

        const auto a0 = -0.5 * y0 + 1.5 * y1 - 1.5 * y2 + 0.5 * y3;
        const auto a1 = y0 - 2.5 * y1 + 2.0 * y2 - 0.5 * y3;
        const auto a2 = -0.5 * y0 + 0.5 * y2;
        return static_cast<Sample>(((a0 * mu + a1) * mu + a2) * mu + y1);
    }

private:
    [[nodiscard]] std::size_t wrappedIndex(long long index) const noexcept {
        const auto size = static_cast<long long>(buffer_.size());
        index %= size;
        if (index < 0) index += size;
        return static_cast<std::size_t>(index);
    }

    std::vector<Sample> buffer_{};
    std::size_t writeIndex_{};
};

} // namespace motionfield
