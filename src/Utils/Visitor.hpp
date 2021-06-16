#pragma once

template <class... Ts>
struct visitor : Ts... { using Ts::operator()...; };
template <class... Ts>
visitor(Ts...) -> visitor<Ts...>;
