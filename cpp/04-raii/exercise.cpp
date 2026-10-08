#include <fcntl.h>
#include <unistd.h>
#include <iostream>

class UniqueFd {
public:
    explicit UniqueFd(int fd = -1)
        : fd_(fd) {}

    ~UniqueFd() {
        // TODO: close owned fd if valid
    }

    UniqueFd(const UniqueFd&) = delete;
    UniqueFd& operator=(const UniqueFd&) = delete;

    int get() const {
        // TODO
        return -1;
    }

    bool valid() const {
        // TODO
        return false;
    }

    int release() {
        // TODO: return owned fd and relinquish ownership without closing
        return -1;
    }

    void reset(int new_fd = -1) {
        // TODO: close old fd when appropriate, then take ownership
    }

private:
    int fd_;
};

int main() {
    int raw = ::open("/dev/null", O_RDONLY);
    if (raw == -1) {
        std::cerr << "open failed\n";
        return 1;
    }

    {
        UniqueFd fd(raw);
        std::cout << "valid: " << fd.valid() << '\n';
        std::cout << "fd: " << fd.get() << '\n';
        int transferred = fd.release();
        std::cout << "after release: " << fd.valid() << '\n';
        fd.reset(transferred);
        std::cout << "after reset: " << fd.valid() << '\n';
    }

    std::cout << "scope ended\n";
    return 0;
}
