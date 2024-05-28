#include <atomic>
#include <iostream>
#include <thread>

struct A {
    float x;
    int y;
    long long z;
};

int main() {
    std::thread t([]() {
        std::cout << "hello world" << std::endl;
    });
    t.join();

    std::atomic<A> a;
    std::cout << std::boolalpha << a.is_lock_free() << std::endl;

    std::atomic<int*> ptr(nullptr);
    int v;
    std::thread producer([&]() {
        int* p = new int(42);
        v = 1024;
        ptr.store(p, std::memory_order_release);
    });
    std::thread consumer([&]() {
        int* p;
        while (!(p = ptr.load(std::memory_order_consume)));
        std::cout << "p: " << *p << std::endl;
        std::cout << "v: " << v << std::endl;
    });

    producer.join();
    consumer.join();



    return 0;
}
