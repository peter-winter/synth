#pragma once

template<typename T>
constexpr inline auto constant(T v)
{
    return [v]{ return v; };
}
