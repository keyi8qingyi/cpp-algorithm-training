#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

class LegacyBuffer {
public:
    explicit LegacyBuffer(std::size_t size)
        : size_(size), data_(nullptr) {
        // TODO: allocate and zero-initialize
    }

    ~LegacyBuffer() {
        // TODO: release resource
    }

    LegacyBuffer(const LegacyBuffer& other)
        : size_(other.size_), data_(nullptr) {
        // TODO: deep copy
    }

    LegacyBuffer& operator=(const LegacyBuffer& other) {
        // TODO: deep copy assignment, handle self-assignment and old resource
        return *this;
    }

    LegacyBuffer(LegacyBuffer&& other) noexcept
        : size_(0), data_(nullptr) {
        // TODO: steal resource and empty other
    }

    LegacyBuffer& operator=(LegacyBuffer&& other) noexcept {
        // TODO: move assignment, handle self-move and old resource
        return *this;
    }

    std::size_t size() const {
        // TODO
        return 0;
    }

    int& at(std::size_t index) {
        // TODO: assume valid index
        return data_[index];
    }

    const int& at(std::size_t index) const {
        // TODO: assume valid index
        return data_[index];
    }

private:
    std::size_t size_;
    int* data_;
};

class ModernBuffer {
public:
    explicit ModernBuffer(std::size_t size)
        // TODO: initialize vector member
    {}

    // Rule of Zero: do not declare destructor/copy/move functions.

    std::size_t size() const {
        // TODO
        return 0;
    }

    int& at(std::size_t index) {
        // TODO
        return data_[index];
    }

    const int& at(std::size_t index) const {
        // TODO
        return data_[index];
    }

private:
    std::vector<int> data_;
};

int main() {
    LegacyBuffer old_a(2);
    old_a.at(0) = 10;
    LegacyBuffer old_b(old_a);
    old_b.at(0) = 20;
    LegacyBuffer old_c(1);
    old_c = old_a;
    LegacyBuffer old_d(std::move(old_b));
    old_c = std::move(old_d);
    old_c = old_c;
    old_c = std::move(old_c);
    std::cout << "legacy: " << old_a.at(0) << ' ' << old_c.at(0) << '\n';

    ModernBuffer modern_a(2);
    modern_a.at(0) = 30;
    ModernBuffer modern_b = modern_a;
    modern_b.at(0) = 40;
    ModernBuffer modern_c = std::move(modern_b);
    std::cout << "modern: " << modern_a.at(0) << ' ' << modern_c.at(0) << '\n';
    return 0;
}
