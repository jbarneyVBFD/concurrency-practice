#include <iostream>
#include <thread>

int main() {

    int id = 0; //define integer variable

    //starting first thread (by reference)
    auto f0 = [&id]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "a) ID in Thread (call by reference) = " << id << std::endl;
    };
    std::thread t1(f0);

    //starting a second thread
    std::thread t2([id]() mutable {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "b) ID in Thread (call by value) = " << id << std::endl;
    });

    //increment and print id in main
    ++id;
    std::cout << "c) ID in main (call by value) = " << id << std::endl;

    //wait for threads before returning
    t1.join();
    t2.join();

    return 0;
}