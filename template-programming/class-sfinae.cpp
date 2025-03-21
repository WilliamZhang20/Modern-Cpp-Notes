#include <iostream>
#include <type_traits>

template<typename T, typename Enable = void>
struct A;

template<typename T>
struct A<T, std::enable_if_t<std::is_signed_v<T>>> {
    
};
template<typename T>
struct A<T, std::enable_if_t<!std::is_signed_v<T>>> {
};

int main() {
    A<int> a1;
    A<unsigned> a2;
}