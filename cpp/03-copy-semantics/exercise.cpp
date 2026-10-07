#include <cstddef>
#include <iostream>

class Buffer {
public:
    explicit Buffer(std::size_t size)
        : size_(size), data_(nullptr) {
        // TODO: allocate size_ integers and initialize them to 0
    }

    ~Buffer() {
        // TODO: release the owned array
    }

    Buffer(const Buffer& other)
        : size_(other.size_), data_(nullptr) {
        // TODO: allocate independent storage and copy every element
    }

    Buffer& operator=(const Buffer& other) {
        // TODO: handle self-assignment

        // TODO: replace this object's old storage with an independent
        // copy of other's storage.

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

void print_buffer(const char* label, const Buffer& buffer) {
    std::cout << label << ':';
    for (std::size_t i = 0; i < buffer.size(); ++i) {
        std::cout << ' ' << buffer.at(i);
    }
    std::cout << '\n';
}

int main() {
    Buffer original(3);
    original.at(0) = 10;
    original.at(1) = 20;
    original.at(2) = 30;

    Buffer copied(original);
    copied.at(0) = 99;

    print_buffer("original", original);
    print_buffer("copied", copied);

    Buffer assigned(1);
    assigned.at(0) = -1;
    assigned = original;
    assigned.at(1) = 88;

    print_buffer("original after assignment", original);
    print_buffer("assigned", assigned);

    // Your assignment operator must survive this.
    assigned = assigned;
    print_buffer("assigned after self-assignment", assigned);

    return 0;
}
