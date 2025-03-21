#include <iostream>

// First example - sum of any number of int array
template<typename... TArgs> // Expands in a template-argument-list, i.e. list of template arguments
int computeIntSum(TArgs... args) { // Typename expansion - the pack expands in an initializer-list, i.e. list of values
    // Sizeof with three dots has to contain parameter packs
    std::cout << "cool printout of sizeof args: " << sizeof...(args) << "\n"; // this outputs the number of elements

    int values[] = {args...}; // Arguments expansion
    
    std::cout << "compared with old fashion: " << sizeof(values) << "\n";
    std::cout << "a single element is size: " << sizeof(values[0]) << "\n";
    int sum = 0;
    for(int i=0; i<sizeof(values)/sizeof(values[0]); i++) {
        sum += values[i];
    }
    return sum;
}

// Alternative: recursive templates for the purpose of adding *ANY* data type
// BASE CASE - prevents infinite expansion!!
template<typename T, typename R> // takes precedence at two arguments due to TEMPLATE OVERLOAD RESOLUTION, where the more specialized template is chosen
auto add(T a, R b) {
    return a + b;
}

// RECURSIVE CASE
// Note: the dots BEFORE the identifier (TArgs typename or args parameter) is a template parameter PACK
template<typename T, typename... TArgs> // Variadic typename
auto add(T a, TArgs... args) { // Typename expansion
    return a + add(args...); // Arguments expansion into separate args - not the dots AFTER the args name
}

struct A {
    int v;
    int f() { return v*v; }
};

template<typename... TArgs>
int f(TArgs... args) {
    // Could have alternatively been any function applied to args - and args doesn't have to be a structs
    return add(args.f()...); // equivalent to: 'A{1}.f(), A{2}.f(), A{3}.f()'
}

template<typename... TArgs>
int g(TArgs... args) {
    return add<std::make_unsigned_t<TArgs>...>(args...);
}

int main() {
    /* NOTE: it is better to run this program side by side with the code */
    std::cout << "for result, expect 55\n";
    std::cout << computeIntSum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << "\n";

    std::cout << "Using recursive templates: " << add(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << "\n";

    std::cout << "adding a bunch of chars: ";
    std::cout << add('a', '1', 'b', '5', '6', '7') << "\n";

    std::cout << "note: adding a single number is generally invalid, and won't compile here either\n";
    // std::cout << add(1) << "\n"; // complains of no matching function to call - since it's empty

    // struct is a POD type - so aggregate initialization works!
    std::cout << "Adding squares of arguments: " << f(A{1}, A{2}, A{3}, A{4}) << "\n";
}
