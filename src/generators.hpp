#pragma once

#include "frequency.hpp"
#include "strong_types.hpp"

#include <cmath>
#include <array>

template <sample_rate Sr, typename Frequency>
class sine_wave
{
public:
    explicit sine_wave(Frequency&& freq):
        freq_(std::move(freq))
    {}
    
    float operator()()
    {
        frequency f = freq_();
        float sample_v = std::sin(static_cast<float>(phase_));
        double increment = TWO_PI * f.hz_ / Sr.value_;
        phase_ += increment;
        if (phase_ >= TWO_PI)
            phase_ -= TWO_PI;
        return sample_v;
    }

private:
    double phase_ = 0.0;
    Frequency freq_;
    
    static constexpr double TWO_PI = 6.28318530717958647692;
};

template <sample_rate Sr, typename Frequency>
class square_wave
{
public:
    explicit square_wave(Frequency&& freq):
        freq_(std::move(freq))
    {}
    
    float operator()()
    {
        frequency f = freq_();
        double period_in_samples = Sr.value_ / f.hz_;
        double half_period = period_in_samples / 2.0;
        
        counter_ += 1.0;
        if (counter_ >= period_in_samples)
            counter_ -= period_in_samples;

        return (counter_ < half_period) ? 1.0f : -1.0f;
    }

private:
    double counter_ = 0.0;
    Frequency freq_;
};


template <sample_rate Sr, typename Frequency>
class saw_wave
{
public:
    explicit saw_wave(Frequency&& freq) : freq_(std::move(freq)) {}

    float operator()()
    {
        frequency f = freq_();
        double increment = f.hz_ / Sr.value_;
        phase_ += 2.0 * increment;
        if (phase_ >= 1.0) phase_ -= 2.0;
        return static_cast<float>(phase_);
    }

private:
    double phase_ = -1.0;
    Frequency freq_;
};
