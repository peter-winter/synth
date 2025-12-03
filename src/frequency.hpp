#pragma once

struct frequency
{
    constexpr explicit frequency(float hz):
        hz_(hz)
    {}
    
    constexpr frequency& operator+=(float hz) noexcept { hz_ += hz; return *this; }
    constexpr frequency& operator-=(float hz) noexcept { hz_ -= hz; return *this; }
    constexpr frequency& operator*=(float x) noexcept { hz_ *= x; return *this; }
    constexpr frequency& operator/=(float x) noexcept { hz_ /= x; return *this; }
    
    float hz_;
};

constexpr frequency operator""_Hz(unsigned long long int v) noexcept
{
    return frequency(static_cast<float>(v));
}

constexpr frequency operator""_Hz(long double v) noexcept
{
    return frequency(static_cast<float>(v));
}

constexpr frequency operator+(frequency f, float hz) noexcept
{
    return frequency(f.hz_ + hz);
}

constexpr frequency operator+(float hz, frequency f) noexcept
{
    return frequency(hz + f.hz_);
}

constexpr frequency operator+(frequency a, frequency b) noexcept
{
    return frequency(a.hz_ + b.hz_);
}

constexpr frequency operator-(frequency f, float hz) noexcept
{
    return frequency(f.hz_ - hz);
}

constexpr frequency operator-(float hz, frequency f) noexcept
{
    return frequency(hz - f.hz_);
}

constexpr frequency operator-(frequency a, frequency b) noexcept
{
    return frequency(a.hz_ - b.hz_);
}

constexpr frequency operator*(frequency f, float x) noexcept
{
    return frequency(f.hz_ * x);
}

constexpr frequency operator*(float x, frequency f) noexcept
{
    return frequency(x * f.hz_);
}

constexpr frequency operator/(frequency f, float x) noexcept
{
    return frequency(f.hz_ / x);
}

constexpr frequency operator/(float x, frequency f) noexcept
{
    return frequency(x / f.hz_);
}

constexpr inline auto const_frequency(frequency f)
{
    return [f]{ return f; };
}
