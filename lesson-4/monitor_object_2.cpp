#include <iostream>
#include <thread>
#include <future>
#include <vector>
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
    
    bool dataIsAvailable()
    {
        std::lock_guard<std::mutex> myLock(_mutex);
        return !_vehicles.empty();
    }

    Vehicle popBack()
    {
        //perform vector modifiction under the lock
        std::lock_guard<std::mutex> uLock(_mutex);

        //remove last vector element from queue
        Vehicle v = std::move(_vehicles.back());
        _vehicles.pop_back();
        _counter --;

        return v; //will be copied due to return value optimization(RVO) in C++
    }

    void puchBack(Vehicle &&v)
    {
        //simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        //perform vector modification under the lock
        std::lock_guard<std::mutex> ulock(_mutex);

        //add vector to the queue
        std::cout << "Vehicle #" << v.getId() << " will be added to the queue" << std::endl;
        _vehicles.emplace_back(std::move(v)); 
        _counter ++;

    }

    int getCounter() { return _counter; }

private:
    std::vector<Vehicle> _vehicles;
    std::mutex _mutex;
    int _counter;
};

int main() 
{
    //create monitor object as a shared pointer to enable access by multiple threads
    std::shared_ptr<WaitingVehicles> queue(new WaitingVehicles);

    std::cout << "Spawning vehicles..." << std::endl;
    
    std::vector<std::future<void>> futures;
    for(int i = 0; i < 10; ++i)
    {
        //create a new vehicle instance and move it into the queue
        Vehicle v(i);
        futures.emplace_back(std::async(std::launch::async, &WaitingVehicles::puchBack, queue, std::move(v)));
    }

    std::cout << "Collecting Results..." << std::endl;

    while(true)
    {
        if (queue->dataIsAvailable())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            Vehicle v = queue->popBack();
            std::cout << "vehicle #" << v.getId() << " has been removed from the queue" << std::endl;

            if(queue->getCounter() <=0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                break;
            }
        }
    }

    std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr){
        ftr.wait();
    });

    std::cout << "Finished processing queue with " << queue->getCounter() << " vehicles remaining" << std::endl;

    return 0;
}