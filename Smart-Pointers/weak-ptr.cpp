// Weak pointer demo
// Why no weak pointer & unique pointer? 
// https://stackoverflow.com/questions/29059343/why-cant-a-weak-ptr-be-constructed-from-a-unique-ptr
#include <iostream>
#include <memory>

class Parent; // Forward declaration avoid recompilation

class Child {
public:
    std::weak_ptr<Parent> parent;  // weak_ptr to avoid circular reference

    ~Child() {
        std::cout << "Child class destructor callled\n";
    }

    void showParent() {
        if (auto p = parent.lock()) {  // Attempt to acquire the shared_ptr from weak_ptr
            std::cout << "Child's parent is alive!" << std::endl;
        } else {
            std::cout << "Parent has been destroyed!" << std::endl;
        }
    }
};

class Parent : public std::enable_shared_from_this<Parent> { // must enable shared to use shared_from_this
public:
    std::shared_ptr<Child> child;  // shared_ptr to Child

    Parent() {
        std::cout << "Parent object created\n";
    }

    ~Parent() {
        std::cout << "Parent object destroyed\n";
    }

    void addChild() { // must be done separately - parent must have been completely built to have child :O
        child = std::make_shared<Child>();
        child->parent = shared_from_this();  // returns a shared pointer as the argument to the weak pointer's construction
    }

    void removeChild() {
        child.reset();
    }
};

int main() {
    {
        // Demo 1 - construction
        auto parent = std::make_shared<Parent>();  // Parent created
        parent->addChild(); // Create child
        std::cout << "Inside scope, both Parent and Child are alive." << std::endl;
        parent->child->showParent();  // Child can access Parent's existence
    }

    // Parent is now out of scope, and both Parent and Child should be destroyed
    // If child's parent pointer was shared - no destructor is called
    // If child's parent pointer is weak - then they are called!

    std::shared_ptr<Child> childPtr; // weak points CANNOT call class methods - so used shared pointer

    {
        // Demo 2 - Deducing locking of weak pointers
        auto parent = std::make_shared<Parent>();  // Parent created
        parent->addChild(); // Create child
        std::cout << "Inside scope, both Parent and Child are alive." << std::endl;
        parent->child->showParent();  // Child can access Parent's existence

        childPtr = parent->child; // this random shared pointer will share ownership with the parent

        parent.reset(); // Will trip the handler in the child class

        childPtr->showParent();
    }

    // The`childPtr` above is fully destroyed at the termination of the program

    return 0;
}