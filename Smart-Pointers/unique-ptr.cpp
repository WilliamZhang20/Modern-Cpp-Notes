// Unique ptr demo - very easy!
#include <iostream>
#include <memory>

class MyClass {
private:
    int x_;
public:
    MyClass(int x = 0) : x_{x} {}
    ~MyClass() {
        std::cout << "Destructor call\n"; // Occurs any time it goes out of scope
    }
    int getValue() { return x_; }
};

int main() {
    int value;
    srand((unsigned)time(0)); 
    value = (rand() % 6) + 1;
    std::unique_ptr<MyClass> ptr1;
    std::cout << "Init address: " << &ptr1 << "\n";
    
    // Note: unique ptr is RAII, as it gets rid of the value when destructed
    ptr1 = std::make_unique<MyClass>(value);
    std::cout << "Initial value: " << ptr1->getValue() << "\n";

    // the copy constructor has been deleted, only move is allowed
    std::unique_ptr<MyClass> ptr2 = std::move(ptr1);
    std::cout << "New address of trashed ptr " << &ptr1 << "\n";
    std::cout << "Value after moving " << ptr2->getValue() << "\n";

    MyClass* rawPtr = ptr2.get(); // retrieve raw version
    std::cout << "raw Ptr address " << rawPtr << "\n";

    std::cout << "raw pointer call " << rawPtr->getValue() << "\n";

    ptr2.reset(); // delete in advance, last destructor call

    // Raw pointer still dangles at original address with random value assigned
    std::cout << "After deletion, raw ptr addr " << rawPtr << "\n";

    // Random value result
    std::cout << "After deletion, raw ptr value " << rawPtr->getValue() << "\n";
    
    // Unique pointer destructor/reset sets to nullptr automatically
    std::cout << "ptr2 address " << &ptr2 << "\n";
    return 0;
}