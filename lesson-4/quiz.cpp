#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <algorithm>
#include <queue>


/*class Vehicle
{
public:
    Vehicle(int id) : _id(id) {}
    int getId() { return _id; }
private:
    int _id;
};
*/
template<class T>
class MessageQueue
{
public:
    
    T recieve()
    {
        //perform vector modification under the lock
        std::unique_lock<std::mutex> uLock(_mutex);
        _cond.wait(uLock, [this]() { return !_messages.empty(); }); //pass unique lock to condition variable

        //remove last vector element from queue
        T t = std::move(_messages.back());
        _messages.pop_back();

        return t; //will not be copied due to return value optimization (RVO) in C++
    }

    void send(T &&t)
    {
        //simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        //perform vector modification under the lock
        std::lock_guard<std::mutex> uLock(_mutex);

        //add vector to the queue
        std::cout << "message " << t << " will be added to the queue" << std::endl;
        _messages.push_back(std::move(t));
        _cond.notify_one(); //notify client after pushing new vehicle into vector
    }

private:
    std::deque<T> _messages;
    std::mutex _mutex;
    std::condition_variable _cond;
};

int main()
{
    //create monitor object as a shared pointer to enable access by multiple threads
    std::shared_ptr<MessageQueue<int>> queue(new MessageQueue<int>);

    std::cout << "Spawning Threads..." << std::endl;
    std::vector<std::future<void>> futures;
    for (int i = 0; i < 10; i++)
    {
        //create a new vehicle instance and move it into the queue
        int message = i;
        futures.emplace_back(std::async(std::launch::async, &MessageQueue<int>::send, queue, std::move(message)));
    }

    std::cout << "Collecting Results..." << std::endl;

    while(true)
    {
        //popBack wakes up when a new element is available in the queue
        int message = queue->recieve();
        std::cout << "message #" << message << " has been removed from the queue" << std::endl;
    }

    std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) {
        ftr.wait();
    });

    std::cout << "Finished" << std::endl;

    return 0;
}