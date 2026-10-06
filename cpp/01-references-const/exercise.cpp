#include <cstddef>
#include <iostream>
#include <vector>

// Task 1: use references. Do not change the signature to pointers.
void swap_values(int& lhs, int& rhs) {
    // TODO: implement
}

// Task 2: choose an appropriate parameter type so the vector is not copied
// and cannot be modified inside this function.
int sum_buffer(/* TODO: parameter */) {
    int sum = 0;

    // TODO: use a range-based for loop

    return sum;
}

class BufferView {
public:
    // Task 3: keep a reference to the external vector instead of copying it.
    explicit BufferView(/* TODO: parameter */)
        /* TODO: initializer list */ {
    }

    std::size_t size() const {
        // TODO: implement
        return 0;
    }

    const int& at(std::size_t index) const {
        // TODO: return a read-only reference to the requested element
        // You may use std::vector::at so invalid indexes are detected.
        throw "TODO";
    }

    void set(std::size_t index, int value) {
        // TODO: modify the underlying vector
    }

private:
    // TODO: declare a reference member bound to the external vector
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
