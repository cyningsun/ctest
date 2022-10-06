#include<chrono>
#include<iostream>
#include<future>
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

int work(std::promise<int> prom) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    prom.set_value(42);
}

int main() 
{
    std::promise<int> prom;
    auto fut = prom.get_future();
    scoped_thread th{work, std::move(prom)};
    std::cout << "I am waiting now\n";
    std::cout<< "Answer: " << fut.get() << "\n";
}
