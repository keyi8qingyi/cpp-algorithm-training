#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

class LegacyBuffer {
public:
    explicit LegacyBuffer(std::size_t size)
        : size_(size), data_(nullptr) {
        data_ = new int[size_]{};
    }

    ~LegacyBuffer() {
        delete[] data_;
    }

    LegacyBuffer(const LegacyBuffer& other)
        : size_(other.size_), data_(nullptr) {
        data_ = new int[size_];
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    LegacyBuffer& operator=(const LegacyBuffer& other) {
        if (this == &other) {
            return *this;
        }

        int* new_data = new int[other.size_];
        for (std::size_t i = 0; i < other.size_; ++i) {
            new_data[i] = other.data_[i];
        }

        delete[] data_;
        data_ = new_data;
        size_ = other.size_;
        return *this;
    }

    LegacyBuffer(LegacyBuffer&& other) noexcept
        : size_(0), data_(nullptr) {
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }

    LegacyBuffer& operator=(LegacyBuffer&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        delete[] data_;
        data_ = other.data_;
        other.data_ = nullptr;
        size_ = other.size_;
        other.size_ = 0;
        return *this;
    }

    std::size_t size() const {
        return size_;
    }

    int& at(std::size_t index) {
        return data_[index];
    }

    const int& at(std::size_t index) const {
        return data_[index];
    }

private:
    std::size_t size_;
    int* data_;
};

class ModernBuffer {
public:
    explicit ModernBuffer(std::size_t size)
        : data_(size)
    {}

    // Rule of Zero: do not declare destructor/copy/move functions.

    std::size_t size() const {
        return data_.size();
    }

    int& at(std::size_t index) {
        return data_[index];
    }

    const int& at(std::size_t index) const {
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
