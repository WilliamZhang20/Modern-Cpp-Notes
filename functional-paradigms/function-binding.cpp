#include <iostream>
#include <functional>

// A simple class with a member function
class Handler {
public:
    void handleEvent(int value) {
        std::cout << "Handling event with value: " << value << std::endl;
    }
};

// A function that takes a callback
void eventTrigger(std::function<void(int)> callback) {
    callback(42);
}

int main() {
    Handler handler;

    srand((unsigned)time(0)); 
    int value = (rand() % 6) + 1;

    // Bind the member function to an instance (have to use the **Pointer** to handler)
    // **Placeholders** is used to indicate the argument, which is not given
    auto boundFunction = std::bind(&Handler::handleEvent, &handler, std::placeholders::_1);

    // Pass the bound function as a callback
    eventTrigger(boundFunction);

    return 0;
}