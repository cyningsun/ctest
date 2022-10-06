#include<chrono>
#include<iostream>
#include<future>
#include<thread>


int work() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 42;
}

int main() 
{
    auto fut = std::async(std::launch::async, work);
    std::cout << "I am waiting now\n";
    std::cout<< "Answer: " << fut.get() << "\n";
}