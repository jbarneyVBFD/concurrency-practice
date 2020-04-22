#include <iostream>
#include <future>
#include <thread>
#include <memory>

class Vehicle
{
public:
    //default constructor
    Vehicle() : _id(0), _name(new std::string("Default Name"))
    {
        std::cout << "Vehicle #" << _id << " default construcor" << std::endl;
    }

    //initializing constructor
    Vehicle(int id, std::string name) : _id(id), _name(new std::string(name))
    {
        std::cout << "Vehicle #" << _id << " initializing constructor called" << std::endl;
    }

    //move constructor with unique pointer
    Vehicle(Vehicle &&src) : _name(std::move(src._name))
    {
        //move id to this and reset id in source
        _id = src.getId();
        src.setId(0);

        std::cout << "Vehicle #" << _id << " move constructor called" << std::endl;
    }

    //setter and getter
    void setId(int id) { _id = id; }
    int getId() { return _id; }
    void setName(std::string name) { *_name = name; }
    std::string getName() { return *_name; }

private:
    int _id;
    std::unique_ptr<std::string> _name;
};

int main()
{
    //create instances of class vehicle
    Vehicle v0; //default constructor
    Vehicle v1(1, "Vehicle 1"); //initializing constructor

    //launch thread that modifies vehicle name
    std::future<void> ftr = std::async([](Vehicle v) {
        v.setName("Vehicle 2");
    }, std::move(v0));

    ftr.wait();
    //std::cout << v0.getName() << std::endl; //this will throw an exception

    return 0;
}