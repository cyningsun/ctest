#include "singleton.hpp"

std::mutex singleton::lock_;
std::atomic<singleton*> singleton::inst_ptr_;

singleton* singleton::instance(/* args */)
{
    singleton* ptr = inst_ptr_.load(std::memory_order_acquire);
    if(ptr == nullptr) {
        std::lock_guard<std::mutex> guard{lock_};
        ptr = inst_ptr_.load(std::memory_order_relaxed);
        if(ptr == nullptr) {
            ptr = new singleton();
            inst_ptr_.store(ptr, std::memory_order_release);
        }
    }
    return inst_ptr_;
}

singleton::~singleton()
{
}
