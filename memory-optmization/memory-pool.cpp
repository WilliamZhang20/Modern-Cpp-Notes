#include <iostream>
#include <vector>
#include <chrono>

template <typename T, std::size_t PoolSize = 1024>
class MemoryPool {
    struct Node {
        Node* next;
    };

    alignas(alignof(T)) char pool[PoolSize * sizeof(T)]; // char for byte-level controls
    Node* freeList;

public:
    MemoryPool() {
        freeList = nullptr;
        for(std::size_t i = 0; i < PoolSize; ++i) {
            // Lower-level, non-heap memory management using void*
            void* addr = pool + i * sizeof(T);
            auto node = reinterpret_cast<Node*>(addr);
            node->next = freeList;
            freeList = node;
        }
    }

    T* allocate() {
        if(!freeList) throw std::bad_alloc();
        Node* node = freeList;
        freeList = freeList->next;
        return reinterpret_cast<T*>(node);
    }

    void deallocate(T* ptr) {
        auto node = reinterpret_cast<Node*>(ptr);
        node->next = freeList;
        freeList = node;
    }
};

// A dummy class with some data
struct MyObject {
    int x, y, z;
    MyObject(int a, int b, int c) : x(a), y(b), z(c) {}
};

int main() {
    constexpr int N = 1000;

    // Benchamrk with normal new/delete
    {
        std::vector<MyObject*> vec;
        vec.reserve(N);

        auto start = std::chrono::high_resolution_clock::now();
        for(int i=0; i<N; ++i) {
            vec.push_back(new MyObject(i, i+1, i+2));
        }
        for(auto ptr : vec) {
            delete ptr;
        }
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Normal new/delete time: "
                  << std::chrono::duration<double>(end - start).count()
                  << " seconds\n";
    }

    // Benchmark with memory pool 
    {
        MemoryPool<MyObject, N> pool;
        std::vector<MyObject*> vec;
        vec.reserve(N);
        auto start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < N; ++i) {
            void* mem = pool.allocate();
            // construct at allocated spot in memory with placement new
            vec.push_back(new(mem) MyObject(i, i+1, i+2));
        }
        for(auto ptr : vec) {
            ptr->~MyObject(); // manually destructor
            pool.deallocate(ptr); // deallocate in "user-defined" space
        }
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Memory pool allocation time: " 
                  << std::chrono::duration<double>(end - start).count()
                  << " seconds\n";
    }
}