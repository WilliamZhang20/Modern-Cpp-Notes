#include <iostream>

// First example - sum of any number of int array
template<typename... TArgs> // Expands in a template-argument-list, i.e. list of template arguments
int computeIntSum(TArgs... args) { // Typename expansion - the pack expands in an initializer-list, i.e. list of values
    // Sizeof with three dots has to contain parameter packs
    std::cout << "cool printout of sizeof args: " << sizeof...(args) << "\n"; // this outputs the number of elements

    int values[] = {args...}; // Arguments expansion
    
    std::cout << "compared with old fashion: " << sizeof(values) << "\n";
    std::cout << "a single element is " << sizeof(values[0]) << "\n";
    int sum = 0;
    for(int i=0; i<sizeof(values)/sizeof(values[0]); i++) {
        sum += values[i];
    }
    return sum;
}

int main() {
    std::cout << "for result, expect 55\n";
    std::cout << computeIntSum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << "\n";
}
