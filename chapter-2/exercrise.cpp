#include <iostream>
#include <map>

template <typename Key, typename Value, typename F>
void update(std::map<Key, Value>& m, F foo) {
    // TODO:
    for (auto&& [key, value] : m) value = foo(key);
}

template <typename... T>
auto avg(T ... t) {
    return (t + ...) / (sizeof...(t));
}

int main() {
    std::map<std::string, long long int> m {
        {"a", 1}, {"b", 2}, {"c", 3} };
    update(m, [](std::string key) -> long long int {
        return std::hash<std::string>{}(key);
    });
    for (auto&& [key, value] : m) {
        std::cout << key << ":" << value << std::endl;
    }

    std::cout << "avg: " << avg(1, 2, 3.5) << std::endl;
}
