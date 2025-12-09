#pragma once

#include "frequency.hpp"

#include <optional>

struct voice
{
    std::optional<frequency> f_;
    bool active_{false};
    uint32_t note_id_;
};

template<typename G>
using to_g_t = std::result_of_t<G(voice*)>;
    
