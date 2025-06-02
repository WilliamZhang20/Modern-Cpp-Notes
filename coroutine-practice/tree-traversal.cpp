#include <iostream>
#include <memory>
#include <coroutine>
#include <optional>
#include <iterator>

// Tree structure
struct Node {
    int value;
    std::unique_ptr<Node> left, right;
    Node(int v) : value(v) {}
};

// Simple forward declaration
template<typename T>
struct GeneratorIterator;

// tree traversal generator
template <typename T>
struct Generator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        std::optional<T> current_value;

        Generator get_return_object() {
            return Generator{handle_type::from_promise(*this)};
        }

        std::suspend_always initial_suspend() { return {}; }

         std::suspend_always final_suspend() noexcept { return {}; }

        std::suspend_always yield_value(T value) {
            // called to give partial results asynchronously
            current_value = value;
            return {};
        }

        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    handle_type coro;

    Generator(handle_type h) : coro(h) {}
    Generator(const Generator&) = delete;
    Generator(Generator&& other) : coro(other.coro) { other.coro = nullptr; }
    ~Generator() { if (coro) coro.destroy(); }

    GeneratorIterator<T> begin() {
        if (coro) coro.resume();
        return GeneratorIterator<T>{coro};
    }

    std::default_sentinel_t end() { return {}; }
};

template<typename T>
struct GeneratorIterator {
    using handle_type = std::coroutine_handle<typename Generator<T>::promise_type>;

    handle_type coro;

    GeneratorIterator(handle_type h) : coro(h) {}

    GeneratorIterator& operator++() {
        coro.resume();
        if (coro.done()) coro = nullptr;
        return *this;
    }

    T operator*() const {
        return coro.promise().current_value.value();
    }

    bool operator==(std::default_sentinel_t) const {
        return !coro || coro.done();
    }

    bool operator!=(std::default_sentinel_t s) const {
        return !(*this == s);
    }
};

Generator<int> inorder(const Node* node) {
    if (!node) co_return;

    for (int v : inorder(node->left.get()))
        co_yield v;

    co_yield node->value;

    for (int v : inorder(node->right.get()))
        co_yield v;
}

int main() {
    // Build tree:
    //       4
    //      / \
    //     2   6
    //    / \ / \
    //   1  3 5  7
    auto root = std::make_unique<Node>(4);
    root->left = std::make_unique<Node>(2);
    root->right = std::make_unique<Node>(6);
    root->left->left = std::make_unique<Node>(1);
    root->left->right = std::make_unique<Node>(3);
    root->right->left = std::make_unique<Node>(5);
    root->right->right = std::make_unique<Node>(7);

    std::cout << "In-order traversal using coroutines: ";
    for (int v : inorder(root.get())) // very simple - you retrieve the range in 1 line
        std::cout << v << " "; // can call anything to it
    std::cout << "\n";
}