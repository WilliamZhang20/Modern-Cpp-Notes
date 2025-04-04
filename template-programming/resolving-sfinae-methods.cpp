#include <iostream>
#include <type_traits>
#include <utility>

/* 
Summary: using true_type and false_type type traits to resolve whether a class has implemented a certain method
*/

// General defaulting for methods that do not satisfy
template <typename T, typename Enable = void>
struct has_foo : std::false_type {};

// Methods of any PUBLIC foo
template <typename T>
struct has_foo<T, std::void_t<decltype(std::declval<T>().foo())>> : std::true_type {};

// More specific to a specific return type
template <typename T>
struct has_foo {};

template <typename T>
inline constexpr bool has_foo_v = has_foo<T>::value;

class A { 
public:
    void foo() {
        std::cout << "foo\n";
    }
};

class B {
public:
    void bar() {
        std::cout << "bar\n";
    }
};

class C {
public:
    double foo() {
        std::cout << "foo\n";
        return 0;
    }
};

int main() {
    std::cout << std::boolalpha;
    std::cout << has_foo_v<A> << "\n";
    std::cout << has_foo_v<B> << "\n";
    std::cout << has_foo_v<C> << "\n";
}