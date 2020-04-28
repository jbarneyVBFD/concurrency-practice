#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx1, mtx2;

void ThreadA()
{
    //creates deadlock problem
    mtx1.lock();
    std::cout << "Thread A" << std::endl;
    mtx2.lock();
    mtx2.unlock();
    mtx1.unlock();
}

void ThreadB()
{
    //creates deadlock problem
    mtx1.lock();
    std::cout << "Thread B" << std::endl;
    mtx2.lock();
    mtx1.unlock();
    mtx2.unlock();
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