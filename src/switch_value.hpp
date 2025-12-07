#pragma once

#include "timeline.hpp"
#include "utility.hpp"

class switch_value
{
public:
    void set_timeline(timeline* tl) noexcept
    {
        t_ = tl;
    }
    
    float operator()()
    {
        if (!t_)
            return 0.0f;

        auto events = t_->get_events();
        for (const auto& ev : events)
        {
            std::visit(overloaded{
                [this](const note_on& on)  { latched_id_ = on.id; active_ = true; },
                [this](const note_off& off){ if (active_ && off.id == latched_id_) active_ = false; }
            }, ev.payload);
        }

        return active_ ? 1.0f : 0.0f;
    }
    
private:
    bool active_{false};
    note_id latched_id_ = 0;
    timeline* t_;
};
