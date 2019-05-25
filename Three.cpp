#include <iostream>

template <size_t B, size_t E>
struct Pow {
    static constexpr size_t value = B * Pow<B, E - 1>::value;
};

template <size_t B>
struct Pow<B, 0> {
    static constexpr size_t value = 1;
};

template <char H, char... A>
struct Three {
    static constexpr size_t value = Three<H>::value * Pow<3, sizeof...(A)>::value + Three<A...>::value;
};

template <char Last>
struct Three<Last> {
    static_assert(Last == '0' || Last == '1' || Last == '2', "digits are either 0, 1 or 2");
    static constexpr size_t value = Last - '0';
};

template <char... chars>
constexpr size_t operator""_three() {
    return Three<chars...>::value;
}

int main()
{
    std::cout << 2210_three << std::endl;
    return 0;
}
