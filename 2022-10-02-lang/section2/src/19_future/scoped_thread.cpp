#include <utility>
#include<chrono>
#include<iostream>
#include<mutex>
#include<thread>

class scoped_thread 
{
public:
    template<typename... Arg>
    scoped_thread(Arg&&... arg):thread_(std::forward<Arg>(arg)...) {}

    scoped_thread(scoped_thread&& other): thread_(std::move(other.thread_)) {}
    
    scoped_thread(const scoped_thread&) = delete;

    ~scoped_thread()
    {
        if(thread_.joinable()) {
            thread_.join();
        }
    }

private:
    std::thread thread_;
};

std::mutex output_lock;

void func(const char* name)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> guard{output_lock};

    std::cout << "I am thread " << name << "\n";
}

int main() 
{
    scoped_thread t1{func, "A"};
    scoped_thread t2{func, "B"};
}