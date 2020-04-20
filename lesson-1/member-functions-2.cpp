#include <iostream>
#include <thread>
#include <string>

class Vehicle
{
public:
    Vehicle() : _id(0) {}
    void addID(int id) { _id = id; }
    void printID() {
        std::cout << "Vehicle ID = " << _id << std::endl;
    }
    void setName(std::string name) { _name = name; }
    void printName() {
        std::cout << "Vehicle Name = " << _name << std::endl;
    }
private:
    int _id;
    std::string _name;
};

int main() 
{
    //create thread
    std::shared_ptr<Vehicle> v(new Vehicle);
    std::thread t = std::thread(&Vehicle::addID, v, 1); //call member function on object v

    
    std::thread t1 = std::thread(&Vehicle::setName, v, "genk");

    //wait for thread to finish
    t.join();
    t1.join();

    //print vehicle id
    v->printID();
    //print vehicle name
    v->printName();

    return 0;
}