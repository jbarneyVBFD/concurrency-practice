#include <iostream>
#include <thread>
#include <future>

class Vehicle
{
public:
    //default constructor
    Vehicle() : _id(0)
    {
        std::cout << "Vehicle #" << _id << " Default constructor called" << std::endl;
    }

    //initializing constructor
    Vehicle(int id) : _id(id)
    {
        std::cout << "Vehicle #" << _id << " Initializing constructor called" << std::endl;
    }

    //setter and getter
    void setId(int id) { _id = id; }
    int getId() { return _id; }

private:
    int _id;

};

int main()
{
    //create instances of class vehicle
    Vehicle v0; //default constructor
    Vehicle v1(1); //initializing constructor

    //read and write name in different threads (which one of the above creates a data race?)
    std::future<void> ftr = std::async([](Vehicle v) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); //simulate work
        v.setId(2);
    }, v0);

    v0.setId(3);

    ftr.wait();
    std::cout << "Vehicle #" << v0.getId() << std::endl;

    return 0;
}