#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <algorithm>

class Vehicle
{
public:
    Vehicle(int id) : _id(id) {}
    int getId() { return _id; }
private:
    int _id;
};

class WaitingVehicles
{
public:
    WaitingVehicles() {}
    
    //getters and setters
    void printSize()
    {
        _mutex.lock();
        std::cout << "#vehicles = " << _vehicles.size() << std::endl;
        _mutex.unlock();
    }


    void pushBack(Vehicle &&v) 
    {
        for (int i = 0; i < 3; ++i)
        {
            if (_mutex.try_lock_for(std::chrono::milliseconds(100)))
            {
                _vehicles.emplace_back(std::move(v));
                std::this_thread::sleep_for(std::chrono::microseconds(100));
                _mutex.unlock();
                break;
            }
            else
            {
                std::cout << "Error at vehicle " << v.getId() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                
            }
            
        }
    }
    
private:
    std::vector<Vehicle> _vehicles;
    std::timed_mutex _mutex;
};

int main()
{
    std::shared_ptr<WaitingVehicles> queue(new WaitingVehicles);
    std::vector<std::future<void>> futures;
    for (int i = 0; i < 1000; ++i )
    {
        Vehicle v(i);
        futures.emplace_back(std::async(std::launch::async, &WaitingVehicles::pushBack, queue, std::move(v)));
    }
    std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) {
        ftr.wait();
    });

    queue->printSize();

    return 0;
}