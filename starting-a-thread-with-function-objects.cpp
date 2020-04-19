#include <iostream>
#include <thread>

class Vehicle {
public: 
    void operator()() {
        std::cout << "Vehicle object has been created \n" << std::endl;
    }
};

int main() {
    //create thread
    //std::thread t(Vehicle()); <- throws an error because the compiler consders it to be a 
    //function declaration that returns an object of type std::thread and has a single unnamed 
    //parameter that is a pointer to function returning an object of type Vehicle

    //the following are solutions allowing the compiler to consider them to be variable definitions
    //for variable t of class std::thread, initialized with an anonymous instance of class Vehicle

    std::thread t1( (Vehicle()) ); // Add an extra pair of paranthesis

    std::thread t2 = std::thread( Vehicle() ); // Use copy initialization

    std::thread t3{ Vehicle() }; // Use uniform initialization with braces 

    //do something in main
    std::cout << "Finished work in main \n";

    //wait for thread to finish
    t1.join();
    t2.join();
    t3.join();

    return 0;
}