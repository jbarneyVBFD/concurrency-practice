#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <algorithm>

double result;
std::mutex mtx;

void printResult(int denom)
{
    //mtx.lock();
    std::cout << "for denom = " << denom << ", the result is " << result << std::endl;
    //mtx.unlock();
}

void divideByNumber(double num, double denom)
{
    mtx.lock();
    try
    {
        //divide num by denom but throw an exception if division by zero is attempted
        if (denom != 0) 
        {
            //mtx.lock();
            result = num / denom;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            //mtx.unlock();
            printResult(num);
            //mtx.unlock();
        }
        else
        {
            throw std::invalid_argument("Exception from thread: Division by zero");
        }
    }
    catch (const std::invalid_argument &e)
    {
        //notify the user about the exception and return
        std::cout << e.what() << std::endl;
        return;
    }
    mtx.unlock();
}

int main() 
{
    //create a number of threads which execute the funciton divide by number with varying parameters
    std::vector<std::future<void>> futures;
    for (double i = -5; i < +5; ++i)
    {
        futures.emplace_back(std::async(std::launch::async, divideByNumber, 50.0, i));
    }

    //wait for the results
    std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) {
        ftr.wait();
    });

    return 0;
}
