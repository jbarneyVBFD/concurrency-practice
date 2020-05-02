#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx1, mtx2;

void ThreadA()
{
    //creates deadlock problem
    std::lock_guard<std::mutex> lock2(mtx2);
    std::cout << "Thread A" << std::endl;
    std::lock_guard<std::mutex> lock1(mtx1);
}

void ThreadB()
{
    //creates deadlock problem
    std::lock_guard<std::mutex> lock1(mtx1);
    std::cout << "Thread B" << std::endl;
    std::lock_guard<std::mutex> lock2(mtx2);
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