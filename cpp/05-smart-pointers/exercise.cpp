#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Session {
public:
    explicit Session(std::string name) : name_(std::move(name)) {
        std::cout << "create: " << name_ << '\n';
    }

    ~Session() {
        std::cout << "destroy: " << name_ << '\n';
    }

    const std::string& name() const {
        return name_;
    }

private:
    std::string name_;
};

// Task 1: create a Session with exclusive ownership.
std::unique_ptr<Session> make_session(std::string name) {
    // TODO: create and return a unique_ptr
    return std::make_unique<Session>(std::move(name));
}

class SessionManager {
public:
    void add(std::unique_ptr<Session> session) {
        // TODO: transfer ownership into sessions_
        sessions_.push_back(std::move(session));
    }

    std::size_t count() const {
        // TODO
        return sessions_.size();
    }

    void print_all() const {
        // TODO: print each session name, one per line
        for(const auto& session : sessions_)
        {
            std::cout<<session->name()<<"\n";
        }
    }

private:
    std::vector<std::unique_ptr<Session>> sessions_;
};

class SharedSessionRegistry {
public:
    void publish(std::shared_ptr<Session> session) {
        // TODO: keep shared ownership
        current_ = session;
    }

    std::weak_ptr<Session> observe() const {
        // TODO: return a non-owning observer
        auto ptr = std::weak_ptr<Session>(current_);
        return ptr;
    }

    void clear() {
        // TODO: relinquish registry ownership
        current_.reset();
    }

private:
    std::shared_ptr<Session> current_;
};

int main() {
    SessionManager manager;
    auto owned = make_session("radius-client");
    manager.add(std::move(owned));
    std::cout << "owned after move: " << (owned ? "yes" : "no") << '\n';
    std::cout << "count: " << manager.count() << '\n';
    manager.print_all();

    std::weak_ptr<Session> observer;
    {
        SharedSessionRegistry registry;
        auto shared = std::make_shared<Session>("monitor");
        registry.publish(shared);
        observer = registry.observe();
        std::cout << "alive while shared exists: "
                  << (observer.lock() ? "yes" : "no") << '\n';
        shared.reset();
        registry.clear();
        std::cout << "alive after clear: "
                  << (observer.lock() ? "yes" : "no") << '\n';
    }
    return 0;
}
