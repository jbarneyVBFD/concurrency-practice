#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <algorithm>

std::mutex mtx;
double result;

void printResult(int denom)
{
    std::cout << "for denom = " << denom << ", the result is " << result << std::endl;
}

void divideByNumber(double num, double denom) 
{
    std::unique_lock<std::mutex> lck(mtx);
    try
    {
        //divide num by denom unless denom is 0, throw an error
        if (denom != 0)
        {
            result = num / denom;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            printResult(denom);
            lck.unlock();

            //do something outside of the lock
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            lck.lock();
            //do something else under the lock
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        else
        {
            throw std::invalid_argument("Exception from thread, division by zero");
        }
        
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << std::endl;
        return;
    }

}

int main()
{
    //create a number of threads that execute the divideByNumber function
    std::vector<std::future<void>> futures;
    for (double i = -5; i < +5; ++i)
    {
        futures.emplace_back(std::async(std::launch::async, divideByNumber, 50.0, i));
    }

    //wait for all threads
    std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) {
        ftr.wait();
    });

    return 0;
}