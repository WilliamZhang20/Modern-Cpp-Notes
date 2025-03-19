// Shared ptr demo:
#include <iostream>
#include <memory>

class MyClass {
private:
    int x_;
public:
    MyClass(int x = 0) : x_{x} {
        std::cout << "Constructor call\n";
    }
    ~MyClass() {
        std::cout << "Destructor call\n"; // For shared pointer, occurs @ zero reference count
    }
    int getValue() { return x_; }
    void incrementValue() { x_++; }
};

void increaseValue(std::shared_ptr<MyClass> ptr, int amount) {
	  // Calls copy constructor of the shared pointer - so increased reference count
	  // Passes value of address...then creates copy in new pointer
    std::cout << "Function call ref count: " << ptr.use_count() << "\n"; // note the dot operator
    for(int i=0; i<amount; i++) {
        ptr->incrementValue();
    }
}

int main() {
    int value;
    srand((unsigned)time(0)); 
    value = (rand() % 6) + 1;
    std::shared_ptr<MyClass> ptr1 = std::make_shared<MyClass>(value);

    std::cout << "Initial pointer value: " << ptr1->getValue() << "\n";
    increaseValue(ptr1, 5);

    // Shared pointer releases shared resource but does not destroy it
    std::cout << "Post function call ref count: " << ptr1.use_count() << "\n";

    std::cout << "After increment by value " << ptr1->getValue() << "\n";
    return 0;
}