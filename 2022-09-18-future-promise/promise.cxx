#include<iostream>
#include<functional>
#include<thread>
#include<future>

void print_int(std::future<int>& fut) 
{
    int x = fut.get();
    std::cout << "value:" << x << "\n";
}


int main()
{
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    prom.set_value(10);

    std::thread t(print_int, std::ref(fut));
    t.join();

    return 0;
}