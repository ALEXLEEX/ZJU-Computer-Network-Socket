#include <iostream>
#include <thread>
#include <string>
#include <system_error>

class test {
public:
    void worker(int id) {
        for (int i = 0; i < 50; i++) {
            std::cout << id << std::endl;
        }
    }
    void startThread(int id) {
        try {
            std::thread t(&test::worker, this, id);
            t.detach();
        } catch (const std::system_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

void run() {
    test t;
    t.startThread(1);
    t.startThread(2);
}

int main()
{
    try {
        std::thread t(run);
        t.join();
    } catch (const std::system_error& e) {
        std::cerr << e.what() << std::endl;
    }
}