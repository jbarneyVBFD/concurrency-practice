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
    
    Vehicle popBack()
    {
        //perform vector modification under the lock
        std::unique_lock<std::mutex> uLock(_mutex);
        _cond.wait(uLock, [this]() { return !_vehicles.empty(); }); //pass unique lock to condition variable

        //remove last vector element from queue
        Vehicle v = std::move(_vehicles.back());
        _vehicles.pop_back();

        return v; //will not be copied due to return value optimization (RVO) in C++
    }

    void pushBack(Vehicle &&v)
    {
        //simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        //perform vector modification under the lock
        std::lock_guard<std::mutex> uLock(_mutex);

        //add vector to the queue
        std::cout << "vehicle #" << v.getId() << " will be added to the queue" << std::endl;
        _vehicles.push_back(std::move(v));
        _cond.notify_one(); //notify client after pushing new vehicle into vector
    }

private:
    std::vector<Vehicle> _vehicles;
    std::mutex _mutex;
    std::condition_variable _cond;
};

int main()
{
    //create monitor object as a shared pointer to enable access by multiple threads
    std::shared_ptr<WaitingVehicles> queue(new WaitingVehicles);

    std::cout << "Spawning Threads..." << std::endl;
    std::vector<std::future<void>> futures;
    for (int i = 0; i < 10; i++)
    {
        //create a new vehicle instance and move it into the queue
        Vehicle v(i);
        futures.emplace_back(std::async(std::launch::async, &WaitingVehicles::pushBack, queue, std::move(v)));
    }

    std::cout << "Collecting Results..." << std::endl;

    while(true)
    {
        //popBack wakes up when a new element is available in the queue
        Vehicle v = queue->popBack();
        std::cout << "vehicle #" << v.getId() << " has been removed from the queue" << std::endl;
    }

    std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) {
        ftr.wait();
    });

    std::cout << "Finished" << std::endl;

    return 0;
}