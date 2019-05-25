#include <iostream>
#include <vector>
#include <type_traits>

//------------------------IsDerived----------------------------------------
class B {};
class D : public B {};


template <class Base, class Derived>
class IsDerived {
    private:

        static constexpr bool helper(void*) {
            return false;
        }

        static constexpr bool helper(Base*) {
            return true;
        }

    public:

        static constexpr bool value = helper(static_cast<Derived*>(nullptr));
};


//------------------------IsSame----------------------------------------
template <typename A, typename B>
struct IsSame {

        static constexpr bool value = false;
};

template <typename A>
struct IsSame <A, A> {

        static constexpr bool value = true;
};

//-----------------------------IsFunction---------------------------------
template <typename T>
struct IsFunction {

    static constexpr bool value = false;
};

template <class R, class...P>
struct IsFunction<R(P...)> {

    static constexpr bool value = true;
};


//-----------------------------IsArray------------------------------------
template <typename T>
struct IsArray {

    static constexpr bool value = false;
};

template <typename T, size_t N>
struct IsArray<T[N]> {

    static constexpr bool value = true;
};


//-------------------------------RemoveExtent-------------------------------
template<typename T>
struct RemoveExtent { };

template<typename T, size_t N>
struct RemoveExtent<T[N]> {

    using type = T;
};

//-------------------------------RemoveAllExtents-------------------------------
template<typename T>
struct RemoveAllExtentsHelper {

    using type = T;
};

template<typename T, size_t N>
struct RemoveAllExtentsHelper<T[N]> {

    using type = typename RemoveAllExtentsHelper<T>::type;
};

template<typename T>
struct RemoveAllExtents {

    using type = typename RemoveAllExtentsHelper<T>::type;
};

//-------------------------------ArrayDimensions-------------------------------
template<typename T, size_t DIM>
struct ArrayDimensionsHelper {
    static constexpr size_t value = DIM;
};

template<typename T, size_t N, size_t DIM>
struct ArrayDimensionsHelper<T[N], DIM> {
    static constexpr size_t value = ArrayDimensionsHelper<T, DIM + 1>::value;
};

template<typename T>
struct ArrayDimensions {
    static constexpr size_t value = ArrayDimensionsHelper<T, 0>::value;
};

//--------------------------------IsIterable---------------------------------------
template <typename...>
using void_t = void;

template<typename, typename U = void>
struct IsIterable {
    static constexpr bool value = false;
};

template<typename T>
struct IsIterable<T, void_t<decltype(std::begin(std::declval<T>()))>> {
    static constexpr bool value = true;
};

//--------------------------------IsCopiable---------------------------------------

template<typename, typename U = void>
struct IsCopiable {
    static constexpr bool value = false;
};

template<typename T>
struct IsCopiable<T, void_t<decltype(T(*static_cast<T*>(nullptr)))>> {
    static constexpr bool value = true;
};



int main()
{
    std::cout << IsDerived<B, D>::value;
    std::cout << IsDerived<B, std::ostream>::value;

    std::cout << IsSame<int, int32_t>::value;
    std::cout << IsSame<int, int64_t>::value;

    std::cout << IsArray<int[22]>::value;
    std::cout << IsArray<char*>::value;

    std::cout << IsFunction<int(int&, char*)>::value;
    std::cout << IsFunction<double>::value;

    std::cout << IsSame<RemoveExtent<int[12][12]>::type, int[12]>::value;

    std::cout << IsSame<RemoveAllExtents<int[12][12]>::type, int>::value;

    std::cout << ArrayDimensions<int[22][13][10]>::value;

    std::cout << IsIterable<int>::value;
    std::cout << IsIterable<std::vector<double>>::value;

    std::cout << IsCopiable<std::string>::value;
    std::cout << IsCopiable<std::ostream>::value;

    return 0;
}
