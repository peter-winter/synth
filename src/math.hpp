#pragma once

constexpr inline float perceptual_gain(float v) noexcept
{
    if (v <= 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    
    // Best perceptual curve: v^4 (or v^5), 0.1^4 = 0.0001, -80 dB
    return v * v * v * v;
}

enum class polyphony_scale : bool
{
    equal_amplitude = false,   // divide-by-N
    equal_power     = true     // divide-by-sqrt(N), supersaw mode
};

template <size_t VoiceCount, polyphony_scale Mode = polyphony_scale::equal_amplitude>
struct polyphony_gain_t
{
    static constexpr float value = (VoiceCount <= 1)
            ? 1.0f
            : (Mode == polyphony_scale::equal_power)
                ? 1.0f / std::sqrt(static_cast<float>(VoiceCount))
                : 1.0f / static_cast<float>(VoiceCount);
};
