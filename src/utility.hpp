#pragma once

template<class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

template<typename G>
using to_g_t = std::result_of_t<G(timeline*)>;

