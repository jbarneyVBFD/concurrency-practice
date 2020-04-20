#include <iostream>
#include <thread>
#include <future>

class Vehicle
{
public:
    //default constructor
    Vehicle() : _id(0), _name(new std::string("Default Name"))
    {
        std::cout << "Vehicle #" << _id << " default constructor called" << std::endl;
    }
    //initializer constructor
    Vehicle(int id, std::string name) : _id(id), _name(new std::string(name))
    {
        std::cout << "Vehicle #" << _id << " initializing constructor called" << std::endl;
    }

    //copy constructor
    Vehicle(Vehicle const &src)
    {
        //Quiz: student code starts here
        _id = src._id;
        if (src._name != nullptr)
        {
            _name = new std::string;
            *_name = *src._name;
        }
        //Quiz: student code ends here
        std::cout << "Vehicle #" << _id << " copy constructor called" << std::endl;
    };

    //setter and getter
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
    //create instance of class vehicle
    Vehicle v0; //default constructor
    Vehicle v1(1, "Vehicle 1"); //initializer constructor

    //launch a thread that modifies the vehicle name
    std::future<void> ftr = std::async([](Vehicle v) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        v.setName("Vehicle 2");
    }, v0);

    v0.setName("Vehicle 3");

    ftr.wait();
    std::cout << v0.getName() << std::endl;

    return 0;
}