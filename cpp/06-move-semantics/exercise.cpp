#include <cstddef>
#include <iostream>
#include <utility>

class MovableBuffer {
public:
    explicit MovableBuffer(std::size_t size)
        : size_(size), data_(nullptr) {
        // TODO: allocate size integers initialized to zero
    }

    ~MovableBuffer() {
        // TODO: release owned memory
    }

    MovableBuffer(const MovableBuffer&) = delete;
    MovableBuffer& operator=(const MovableBuffer&) = delete;

    MovableBuffer(MovableBuffer&& other) noexcept
        : size_(0), data_(nullptr) {
        // TODO: take ownership from other and leave other empty
    }

    MovableBuffer& operator=(MovableBuffer&& other) noexcept {
        // TODO: handle self-move and existing resource
        // TODO: transfer ownership and leave other empty
        return *this;
    }

    std::size_t size() const {
        // TODO
        return 0;
    }

    int* data() {
        // TODO
        return nullptr;
    }

    const int* data() const {
        // TODO
        return nullptr;
    }

    void set(std::size_t index, int value) {
        // TODO: assume index is valid
    }

    int get(std::size_t index) const {
        // TODO: assume index is valid
        return 0;
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
