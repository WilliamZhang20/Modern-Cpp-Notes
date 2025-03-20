#include <iostream>
#include <type_traits>

template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type
printType(T value) {
    std::cout << "Integral type: " << value << std::endl;
}

template<typename T>
typename std::enable_if<!std::is_integral<T>::value>::type
printType(T value) {
    std::cout << "Non-integral type: " << value << std::endl;
}

template<typename T>
// Deduction (checking template argument) is BEFORE overload resolution (with enable_if_t, removing functions from overload set)
std::enable_if_t<std::is_signed_v<T>> f(T) { // the function parameter is T, deduced from argument
    std::cout << "signed\n";
}
template<typename T>
std::enable_if_t<!std::is_signed_v<T>> f(T) {
    std::cout << "unsigned\n";
}

int main() {
    printType(42);      // This will print "Integral type: 42"
    printType(3.14);    // This will print "Non-integral type: 3.14"

    f(1); // a f<float>(u) would have also worked
    f(1u);
    return 0;
}
