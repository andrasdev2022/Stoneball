#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <array>

namespace Algorithms {
/*
template <typename First, typename Second>
bool anyOf(First&& first, Second&& second) {
    return first == second;
}

template <typename V, typename First, typename... Rest>
bool anyOf(V&& val, First&& first, Rest... rest) {
    return val == first || anyOf(std::forward<V>(val), std::forward<Rest>(rest)...);
}
*/

template <typename... V>
bool anyOf(V&&... values) {
    std::array<std::common_type_t<V...>,sizeof...(V)> array = { std::forward<V>(values)... };
    for(size_t i = 1; i < array.size(); ++i) {
        if(array[0] == array[i]) {
            return true;
        }
    }
    return false;
}

template <typename V, typename... Rest>
bool noneOf(V&& val, Rest... rest) {
    return !anyOf(std::forward<V>(val), std::forward<Rest>(rest)...);
}

} // namespace Algorithms

#endif // ALGORITHMS_H
