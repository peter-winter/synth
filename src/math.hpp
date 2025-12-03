#pragma once

constexpr inline float perceptual_gain(float v) noexcept
{
    if (v <= 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    
    // Best perceptual curve: v^4 (or v^5), 0.1^4 = 0.0001, -80 dB
    return v * v * v * v;
}
