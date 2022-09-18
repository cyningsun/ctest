#include<iostream>
#include<functional>
#include<thread>
#include<future>


int main()
{
    std::packaged_task<int()> task([](){ return 7; });
    std::thread t(std::ref(task)); 
    std::future<int> fut = task.get_future(); 

    t.join();

    int x = fut.get();
    std::cout << "value:" << x << "\n";
    

    return 0;
}