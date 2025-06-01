// Type erasure in C++ with templates
// Simple calculator example...
#include <iostream>
#include <vector>
#include <memory>

class CommandInterface { // interface for float commands - notice max size is fixed globally
public:
    virtual ~CommandInterface() = default;
    virtual float execute(float a, float b) = 0;
};

template <typename T>
class CommandWrapper : public CommandInterface {
public:
    CommandWrapper(T cmd) : command(std::move(cmd)) {}
    
    virtual float execute(float a, float b) override {
        return command.execute(a, b);
    }
private:
    T command;
};

class Command {
    std::shared_ptr<CommandInterface> cmd;
public:
    template <typename T>
    Command(T&& action) : cmd(std::make_shared<CommandWrapper<T>>(std::forward<T>(action))) {}

    virtual float execute(float a, float b) {
        return cmd->execute(a, b);
    } 
};

class AddNumbers {
public:
    float execute(float a, float b) {
        return a + b;
    }
};

class MultiplyNumbers {
public:
    float execute(float a, float b) {
        return a * b;
    }
};

int main() {
    std::vector<Command> commands;
    std::vector<std::vector<float>> args = {{3, 4}, {5, 6}, {7, 8}};
    commands.emplace_back(AddNumbers{});
    commands.emplace_back(MultiplyNumbers{});
    commands.emplace_back(MultiplyNumbers{});
    for (int i=0; i<commands.size(); i++) {
        std::cout << commands[i].execute(args[i][0], args[i][1]) << std::endl;  // Polymorphic call via type erasure
    }
    return 0;
}
