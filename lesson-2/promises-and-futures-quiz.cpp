#include <iostream>
#include <thread>
#include <future>
#include <cmath>

void computeSqrt(std::promise<double> &&prms, double input)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); //simulate work
    double output = sqrt(input);
    prms.set_value(output);
}

int main()
{
    //define input data
    double inputData = 42;

    //create promise and future
    std::promise<double> prms;
    std::future<double> ftr = prms.get_future();

    //start thread and pass promise as argument
    std::thread t(computeSqrt, std::move(prms), inputData);

    //Student task starts here
    //wait for result to become available
    auto status = ftr.wait_for(std::chrono::milliseconds(1000));
    if (status == std::future_status::ready ) //result is ready
    std::cout << "Result = " << ftr.get() << std::endl;

    //print error if not available
    else if (status == std::future_status::timeout || status == std::future_status::deferred) //thread timed out before result retrieved
    std::cout << "error, timeout before able to retrieve sqrt" << std::endl;

    //Student task ends here

    //thread barrier
    t.join();


    return 0;
}