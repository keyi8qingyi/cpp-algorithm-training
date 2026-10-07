#include <iostream>
#include <string>

class Connection {
public:
    // Task 1:
    // Initialize every member with a member initializer list.
    explicit Connection(int id, std::string name)
        : id_(id),name_(name),connected_(false)
    {

    }

    // Task 4: observe object lifetime.
    ~Connection() {
        // TODO: print "destroy connection: <id>"
        std::cout<<"destroy connection: "<< id_<<"\n";
    }

    int id() const {
        return id_;
    }

    const std::string& name() const {
        return name_;
    }

    bool is_connected() const {
        return connected_;
    }

    void connect() {
        connected_=true;
    }

    void disconnect() {
        connected_=false;
    }

private:
    const int id_;
    std::string name_;
    bool connected_;
};

void inspect(const Connection& connection) {
    std::cout << "id: " << connection.id() << '\n';
    std::cout << "name: " << connection.name() << '\n';
    std::cout << "connected: "
              << (connection.is_connected() ? "yes" : "no")
              << '\n';
}

int main() {
    std::cout << "enter main scope\n";

    Connection connection(1001, "radius-server");
    inspect(connection);

    connection.connect();
    inspect(connection);

    {
        std::cout << "enter inner scope\n";
        Connection temporary(1002, "temporary");
        temporary.connect();
        inspect(temporary);
        std::cout << "leave inner scope\n";
    }

    connection.disconnect();
    inspect(connection);

    std::cout << "leave main scope\n";
    return 0;
}
