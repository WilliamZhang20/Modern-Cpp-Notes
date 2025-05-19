// Implementing type traits under the hood

// Implementing is_same
#include <iostream>

template <typename T, typename U>
struct is_same : std::false_type {};  // Default is false

template <typename T>
struct is_same<T, T> : std::true_type {};  // Specialization for the same types

template <typename T, typename U>
constexpr bool is_same_v = is_same<T, U>::value;

// Implementing make_const
template <typename T>
struct make_const {
  using type = const T;
};

// Alias template for convenience
template <typename T>
using make_const_t = typename make_const<T>::type;

int main() {
    // Check if two types are the same
    std::cout << std::boolalpha; // prints true/false instead of 1/0
    std::cout << std::is_same<int, int>::value << std::endl; // true
    std::cout << std::is_same<int, double>::value << std::endl; // false
    std::cout << std::is_same_v<int, double> << std::endl;

    std::cout << "testing make_const\n";

    using const_int = make_const_t<int>;
    const_int ci = 10;
    std::cout << "Type of ci (MakeConst_t<int>): ";
    if (std::is_same_v<const_int, const int>) {
        std::cout << "It's a const int!" << std::endl;
    }
    return 0;
}