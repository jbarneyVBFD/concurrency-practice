#include <iostream>
#include <future>
#include <thread>

class Vehicle
{
public:
    //default constructor
    Vehicle() : _id(0), _name(new std::string("Default Name"))
    {
        std::cout << "Vehicle #" << _id << std::endl;
    }

    //initializing constructor
    Vehicle(int id, std::string name) : _id(id), _name(new std::string(name))
    {
        std::cout << "Vehicle #" << _id << std::endl;
    }

    //copy constructor
    Vehicle(Vehicle const &src)
    {
        _id = src._id;
        if (src._name != nullptr)
        {
            _name = new std::string;
            *_name = *src._name;
        }
        std::cout << "Vehicle #" << _id << " copy constructor called" << std::endl;
    };

    //move constructor
    Vehicle(Vehicle &&src)
    {
        _id = src.getId();
        _name = new std::string(src.getName());

        src.setId(0);
        src.setName("Default Name");

        std::cout << "Vehicle #" << _id << "move constructor called" << std::endl;
    };

    //getter and setter
    void setId(int id) { _id = id; }
    int getId() { return _id; }
    void setName(std::string name) { *_name = name; }
    std::string getName() { return *_name; }

private:
    int _id;
    std::string *_name;
};

int main()
{
    //create instances of class vehicle
    Vehicle v0; //default constructor
    Vehicle v1(1, "Vehicle 1"); //initializing constructor

    //launch a thread that modifies the vehicle name
    std::future<void> ftr = std::async([](Vehicle v) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        v.setName("Vehicle 2");
    }, std::move(v0));

    ftr.wait();
    std::cout << v0.getName() << std::endl;
    
    return 0;
}