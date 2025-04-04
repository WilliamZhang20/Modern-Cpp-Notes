#include <iostream>
#include <vector>

/* 
Summary: 
*/

template<typename T, typename Enable = void>
struct A {
    void function() {
        std::cout << "Regular integer struct\n";
    }
};

template<typename T>
struct A<T, std::enable_if_t<std::is_signed_v<T>>> {
    void function() {
        std::cout << "Signed integer struct\n";
    }
};
template<typename T>
struct A<T, std::enable_if_t<!std::is_signed_v<T>>> {
    void function() {
        std::cout << "unsigned integer struct\n";
    }
};

int main() {
    A<int> a1;
    A<unsigned> a2;
    a1.function();
    a2.function();
}