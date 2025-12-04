#pragma once

#include "constant.hpp"

#include <cmath>
#include <type_traits>
#include <utility>

template <typename G, typename Ceil, typename Release>
class peak_limiter
{
public:
    explicit peak_limiter(G&& g, Ceil&& c = constant(0.95f), Release&& r = constant(0.999f))
        : g_(std::forward<G>(g))
        , c_(std::forward<Ceil>(c))
        , r_(std::forward<Release>(r))
        , peak_(0.0f)
    {}

    float operator()()
    {
        float in = g_();
        float c = c_();
        
        // One-pole peak detector (leaky max)
        float abs_sample = std::abs(in);
        peak_ = std::max(abs_sample, peak_ * r_());

        // Soft gain reduction only when needed
        float gain = (peak_ > c) ? (c / peak_) : 1.0f;
        return in * gain;
    }

private:
    G g_;
    Ceil c_;
    Release r_;
    float peak_;
};
