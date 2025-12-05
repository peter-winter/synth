#pragma once

#include "frequency.hpp"
#include "strong_types.hpp"
#include "math.hpp"

#include <cmath>

template <sample_rate Sr, typename Frequency>
class sine_wave
{
public:
    explicit sine_wave(Frequency freq):
        freq_(freq)
    {}
    
    float operator()()
    {
        frequency f = freq_();
        double increment = math::two_pi * f.hz_ / Sr.value_;
        phase_ += increment;                     // <--- no branch, no fmod
        float sample = std::sin(static_cast<float>(phase_));
        // phase_ is allowed to grow forever — double has ~53 bits mantissa
        // > 200 years before precision loss at 48 kHz
        return sample;
    }

private:
    double phase_ = 0.0;
    Frequency freq_;
};

template <sample_rate Sr, typename Frequency>
class square_wave
{
public:
    explicit square_wave(Frequency freq):
        freq_(freq)
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
    explicit saw_wave(Frequency freq) : freq_(freq) {}

    float operator()()
    {
        frequency f = freq_();
        float sample = std::sin(static_cast<float>(phase_));
        double increment = math::two_pi * f.hz_ / Sr.value_;
        phase_ += increment;
        if (phase_ >= math::two_pi)
            phase_ -= math::two_pi;
        return sample;
    }

private:
    double phase_ = -1.0;
    Frequency freq_;
};

template <sample_rate Sr, typename Frequency>
class down_saw_wave
{
public:
    explicit down_saw_wave(Frequency freq) : freq_(freq) {}

    float operator()()
    {
        frequency f = freq_();
        double increment = f.hz_ / Sr.value_;
        phase_ += increment;
        if (phase_ >= 1.0) phase_ -= 1.0;
        return static_cast<float>(1.0 - phase_);   // +1 → -1 ramp
    }

private:
    double phase_ = 0.0;
    Frequency freq_;
};

template <sample_rate Sr, typename Frequency>
class triangle_wave
{
public:
    explicit triangle_wave(Frequency freq) : freq_(freq) {}

    float operator()()
    {
        frequency f = freq_();
        double increment = 2.0 * f.hz_ / Sr.value_;   // 2x speed because we fold twice
        phase_ += increment;
        if (phase_ >= 1.0) phase_ -= 2.0;
        return static_cast<float>(phase_ < 0.0 ? -phase_ - 1.0 : phase_ - 1.0) * 2.0f - 1.0f;
    }

private:
    double phase_ = 0.0;
    Frequency freq_;
};

class white_noise
{
public:
    white_noise() = default;

    float operator()()
    {
        // Xorshift* (Daniel Lemire / Sebastiano Vigna)
        // Magic constant 0x2545F4914F6CDD1DULL from "Fast Random Integer Generation in an Interval" (PCG paper)
        uint64_t x = state_;
        x ^= x >> 12;
        x ^= x << 25;
        x ^= x >> 27;
        state_ = x;

        // Convert to float in [-1, 1) — extremely high quality uniform distribution
        uint64_t bits = (x * 0x2545F4914F6CDD1DULL) >> 32;
        return static_cast<float>(static_cast<int32_t>(bits)) * (1.0f / 0x80000000);
    }

private:
    uint64_t state_ = 0x853c49e6748fea9bULL;  // fixed nonzero seed
};

class pink_noise
{
public:
    pink_noise() = default;

    float operator()()
    {
        float white = white_gen_();

        b0 = b0 * 0.99886f + white * 0.0555179f;
        b1 = b1 * 0.99332f + white * 0.0750759f;
        b2 = b2 * 0.96900f + white * 0.1538520f;
        b3 = b3 * 0.86650f + white * 0.3104856f;
        b4 = b4 * 0.55000f + white * 0.5329522f;
        b5 = b5 * 0.31000f + white * -0.5329522f;
        b6 = b6 * 0.11500f + white * -0.0963792f;

        return (b0 + b1 + b2 + b3 + b4 + b5 + b6 + white * 0.5362f) * 0.11f;
    }

private:
    white_noise white_gen_;
    float b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0;
};
