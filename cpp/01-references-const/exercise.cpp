#include <cstddef>
#include <iostream>
#include <vector>

// Task 1: use references. Do not change the signature to pointers.
void swap_values(int& lhs, int& rhs) {
    int temp = lhs;
    lhs = rhs;
    rhs = temp;
}

// Task 2: choose an appropriate parameter type so the vector is not copied
// and cannot be modified inside this function.
int sum_buffer(const std::vector<int>& data) {
    int sum = 0;
    for (int value : data) {
        sum += value;
    }
    return sum;
}

class BufferView {
public:
    // Task 3: keep a reference to the external vector instead of copying it.
    explicit BufferView(std::vector<int>& buffer)
        : buffer_(buffer) {
    }

    std::size_t size() const {
        return buffer_.size();
    }

    const int& at(std::size_t index) const {
        return buffer_.at(index);
    }

    void set(std::size_t index, int value) {
        buffer_.at(index) = value;
    }

private:
    std::vector<int>& buffer_;
};

int main() {
    int a = 10;
    int b = 20;
    swap_values(a, b);
    std::cout << "swap: " << a << ' ' << b << '\n';

    std::vector<int> data{1, 2, 3, 4, 5};
    std::cout << "sum: " << sum_buffer(data) << '\n';

    BufferView view(data);
    std::cout << "size: " << view.size() << '\n';
    std::cout << "first: " << view.at(0) << '\n';

    view.set(0, 42);
    std::cout << "modified: " << data.at(0) << '\n';

    return 0;
}
