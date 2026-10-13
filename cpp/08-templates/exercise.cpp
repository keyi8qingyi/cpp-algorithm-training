#include <array>
#include <cstddef>
#include <iostream>
#include <stdexcept>

// Task 1: function template
template <typename T>
T clamp_value(const T& value, const T& low, const T& high) {
    // TODO: return value constrained to [low, high]
    return value;
}

// Task 2: class template with type and non-type parameters
template <typename T, std::size_t N>
class FixedBuffer {
public:
    std::size_t size() const {
        // TODO
        return 0;
    }

    T& at(std::size_t index) {
        // TODO: bounds-checked access
        return data_[index];
    }

    const T& at(std::size_t index) const {
        // TODO: bounds-checked access
        return data_[index];
    }

    void fill(const T& value) {
        // TODO: fill all elements
    }

private:
    std::array<T, N> data_{};
};

// Task 3: reusable typed counter
template <typename T>
class Counter {
public:
    explicit Counter(T initial) : value_(initial) {}

    void increment(const T& delta) {
        // TODO
    }

    T value() const {
        // TODO
        return T{};
    }

private:
    T value_;
};

int main() {
    std::cout << "clamp int: " << clamp_value(12, 0, 10) << '\n';
    std::cout << "clamp double: " << clamp_value(-1.5, 0.0, 1.0) << '\n';

    FixedBuffer<int, 4> packets;
    packets.fill(7);
    packets.at(1) = 42;
    const auto& readonly = packets;
    std::cout << "buffer size: " << packets.size()
              << ", value: " << readonly.at(1) << '\n';

    FixedBuffer<char, 8> flags;
    flags.fill('x');
    std::cout << "flags[0]: " << flags.at(0) << '\n';

    Counter<int> requests(10);
    requests.increment(5);
    Counter<double> latency(1.5);
    latency.increment(0.25);
    std::cout << "requests: " << requests.value()
              << ", latency: " << latency.value() << '\n';
    return 0;
}
