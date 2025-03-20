#include <iostream>
#include <cmath> // For std::fabs

template<typename T>
bool compare(T a, T b) {
    return a < b;
}

template<>
bool compare<float>(float a, float b) { // partial specialization used
    std::cout << "call special. ";
    return std::fabs(a - b) < 1e-9; // need fabs due to imprecisions in computer floating point number storage, so we set expect a small difference rather than perfect equality
}

int main() {
    float a1 = 3.14, b1 = 3.140;
    char a = 'A', b = '9';
    std::cout << "Comparing chars: " << compare(a, b) << "\n";
    std::cout << "comparing floats: " << compare(a1, b1) << "\n";
}