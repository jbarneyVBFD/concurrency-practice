#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx1, mtx2;

void ThreadA()
{
    //ensures that locks are always executed in the same order
    std::lock(mtx1, mtx2);
    std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);
    std::cout << "Thread A" << std::endl;
    std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);
}

void ThreadB()
{
    std::lock(mtx1, mtx2);
    std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);
    std::cout << "Thread B" << std::endl;
    std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);
}

void ExecuteThreads()
{
    std::thread t1( ThreadA );
    std::thread t2( ThreadB );

    t1.join();
    t2.join();

    std::cout << "Finished" << std::endl;
}

int main()
{
    ExecuteThreads();

    return 0;
}