#pragma once

#include "frequency.hpp"

#include <cstdint>
#include <variant>
#include <vector>
#include <ranges>
#include <algorithm>

using sample_index = uint64_t;
using note_id      = int32_t;

struct note_on
{
    note_id id; 
    frequency freq;
};

struct note_off
{
    note_id id;
};

using event = std::variant<note_on, note_off>;

class timeline
{
public:
    struct timed_event
    {
        sample_index when;
        event        payload;
    };

    void note_on_at(sample_index when, note_id id, frequency freq)
    {
        insert(timed_event{when, note_on{id, freq}});
    }

    void note_off_at(sample_index when, note_id id)
    {
        insert(timed_event{when, note_off{id}});
    }

    using events_view = std::ranges::subrange<std::vector<timed_event>::const_iterator>;
        
    auto operator()() -> events_view
    {
        ++current_sample_;

        if (next_event_it_ == events_.end() || current_sample_ < next_event_it_->when)
            return {events_.end(), events_.end()};

        auto upper = std::ranges::upper_bound(
            next_event_it_, events_.end(), current_sample_,
            std::less<>{}, &timed_event::when);

        events_view result{next_event_it_, upper};
        next_event_it_ = upper;
        return result;
    }

    void reset()
    {
        current_sample_ = 0;
        next_event_it_ = events_.begin();
    }

private:
    void insert(const timed_event& te)
    {
        auto it = std::ranges::upper_bound(events_, te.when, std::less<>{}, &timed_event::when);
        events_.insert(it, te);
    }
    
    std::vector<timed_event> events_;
    sample_index current_sample_ = 0;
    std::vector<timed_event>::const_iterator next_event_it_ = events_.begin();

    static inline const auto empty_view = std::views::empty<timed_event>;
};
