#include <utility>
#include<chrono>
#include<iostream>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<functional>

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

void work(std::condition_variable& cv, int& result) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    result = 42;
    cv.notify_one();
}

int main() 
{
    std::condition_variable cv;
    int result;

    scoped_thread th{work, std::ref(cv),  std::ref(result)};
    std::cout << "I am waiting now\n";
    std::mutex cv_mut;
    std::unique_lock<std::mutex> lock{cv_mut};
    cv.wait(lock);
    std::cout<< "Answer: " << result << "\n";
}