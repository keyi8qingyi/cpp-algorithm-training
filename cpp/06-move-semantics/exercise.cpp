#include <cstddef>
#include <iostream>
#include <utility>

class MovableBuffer {
public:
    explicit MovableBuffer(std::size_t size)
        : size_(size), data_(nullptr) {
        data_ = new int[size_]{};
    }

    ~MovableBuffer() {
        delete[] data_;
    }

    MovableBuffer(const MovableBuffer&) = delete;
    MovableBuffer& operator=(const MovableBuffer&) = delete;

    MovableBuffer(MovableBuffer&& other) noexcept
        : size_(0), data_(nullptr) {
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }

    MovableBuffer& operator=(MovableBuffer&& other) noexcept {
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

    int* data() {
        return data_;
    }

    const int* data() const {
        return data_;
    }

    void set(std::size_t index, int value) {
        data_[index] = value;
    }

    int get(std::size_t index) const {
        return data_[index];
    }

private:
    std::size_t size_;
    int* data_;
};

int main() {
    MovableBuffer first(3);
    first.set(0, 10);
    first.set(1, 20);

    MovableBuffer second(std::move(first));
    std::cout << "first size after move: " << first.size() << '\n';
    std::cout << "second[0]: " << second.get(0) << '\n';

    MovableBuffer third(1);
    third.set(0, 99);
    third = std::move(second);
    std::cout << "second size after assignment: " << second.size() << '\n';
    std::cout << "third[1]: " << third.get(1) << '\n';

    third = std::move(third);
    std::cout << "third size after self move: " << third.size() << '\n';
    return 0;
}
