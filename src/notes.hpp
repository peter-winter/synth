#pragma once

#include "frequency.hpp"

// Scientific pitch notation: C4 = middle C, A4 = 440 Hz (12-TET)
// All values are constexpr — computed at compile time
struct notes
{
    static constexpr frequency A4 = 440.0_Hz;

    inline static constexpr frequency compute(int i)
    {
        int semitones_from_A4 = i - 57;
        float exponent = static_cast<float>(semitones_from_A4) / 12.0f;
        return frequency(A4.hz_ * std::pow(2.0f, exponent));
    }

    static constexpr frequency C (int octave) { return compute(octave * 12 +  0); }
    static constexpr frequency Cs(int octave) { return compute(octave * 12 +  1); } // C♯/D♭
    static constexpr frequency D (int octave) { return compute(octave * 12 +  2); }
    static constexpr frequency Ds(int octave) { return compute(octave * 12 +  3); } // D♯/E♭
    static constexpr frequency E (int octave) { return compute(octave * 12 +  4); }
    static constexpr frequency F (int octave) { return compute(octave * 12 +  5); }
    static constexpr frequency Fs(int octave) { return compute(octave * 12 +  6); } // F♯/G♭
    static constexpr frequency G (int octave) { return compute(octave * 12 +  7); }
    static constexpr frequency Gs(int octave) { return compute(octave * 12 +  8); } // G♯/A♭
    static constexpr frequency A (int octave) { return compute(octave * 12 +  9); }
    static constexpr frequency As(int octave) { return compute(octave * 12 + 10); } // A♯/B♭
    static constexpr frequency B (int octave) { return compute(octave * 12 + 11); }

    // Flat aliases (optional but nice)
    static constexpr frequency Db(int octave) { return Cs(octave); }
    static constexpr frequency Eb(int octave) { return Ds(octave); }
    static constexpr frequency Gb(int octave) { return Fs(octave); }
    static constexpr frequency Ab(int octave) { return Gs(octave); }
    static constexpr frequency Bb(int octave) { return As(octave); }
};

// Compile-time sanity checks
static_assert(notes::C(4).hz_  > 261.62f && notes::C(4).hz_  < 261.64f, "C4 frequency wrong");
static_assert(notes::A(4).hz_ == 440.0f, "A4 must be exactly 440 Hz");


