#include "heartbeat.h"
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <chrono>
#include <thread>

// Heartbeat 基类实现
Heartbeat::Heartbeat(const std::string& name, std::chrono::milliseconds interval)
    : name_(name), interval_(interval), running_(false), stats_(name) {
}

Heartbeat::~Heartbeat() {
    stop();
}

void Heartbeat::stop() {
    if (running_) {
        running_ = false;
        if (thread_ && thread_->joinable()) {
            thread_->join();
        }
    }
}

HeartbeatStats& Heartbeat::getStats() {
    return stats_;
}

// CondVarHeartbeat 实现
CondVarHeartbeat::CondVarHeartbeat(std::chrono::milliseconds interval)
    : Heartbeat("CondVar + 绝对时间戳", interval) {
}

CondVarHeartbeat::~CondVarHeartbeat() {
    stop();
}

void CondVarHeartbeat::start(const HeartbeatCallback& callback) {
    if (running_) {
        return;
    }
    
    running_ = true;
    thread_ = std::make_unique<std::thread>([this, callback]() {
        std::unique_lock<std::mutex> lock(mutex_);
        
        while (running_) {
            // 计算下一个心跳的绝对时间
            auto now = std::chrono::steady_clock::now();
            auto next_time = now + interval_;
            
            // 记录心跳事件
            callback(now);
            stats_.recordHeartbeat(now, std::chrono::microseconds(interval_));
            
            // 等待到下一个心跳时间
            cv_.wait_until(lock, next_time, [this]() { return !running_; });
        }
    });
}

#ifdef __linux__
// TimerfdHeartbeat 实现（Linux平台）
TimerfdHeartbeat::TimerfdHeartbeat(std::chrono::milliseconds interval)
    : Heartbeat("Timerfd + epoll", interval), epoll_fd_(-1), timer_fd_(-1) {
}

TimerfdHeartbeat::~TimerfdHeartbeat() {
    stop();
    
    if (timer_fd_ >= 0) {
        close(timer_fd_);
    }
    
    if (epoll_fd_ >= 0) {
        close(epoll_fd_);
    }
}

void TimerfdHeartbeat::start(const HeartbeatCallback& callback) {
    if (running_) {
        return;
    }
    
    // 创建 timerfd
    timer_fd_ = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (timer_fd_ == -1) {
        std::cerr << "Failed to create timerfd: " << strerror(errno) << std::endl;
        return;
    }
    
    // 创建 epoll 实例
    epoll_fd_ = epoll_create1(0);
    if (epoll_fd_ == -1) {
        std::cerr << "Failed to create epoll: " << strerror(errno) << std::endl;
        close(timer_fd_);
        timer_fd_ = -1;
        return;
    }
    
    // 设置定时器
    struct itimerspec timer_spec = {0};
    timer_spec.it_value.tv_sec = interval_.count() / 1000;
    timer_spec.it_value.tv_nsec = (interval_.count() % 1000) * 1000000;
    timer_spec.it_interval = timer_spec.it_value;  // 设置为周期定时器
    
    if (timerfd_settime(timer_fd_, 0, &timer_spec, nullptr) == -1) {
        std::cerr << "Failed to set timerfd: " << strerror(errno) << std::endl;
        close(timer_fd_);
        close(epoll_fd_);
        timer_fd_ = -1;
        epoll_fd_ = -1;
        return;
    }
    
    // 将 timerfd 添加到 epoll 中
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = timer_fd_;
    
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, timer_fd_, &ev) == -1) {
        std::cerr << "Failed to add timerfd to epoll: " << strerror(errno) << std::endl;
        close(timer_fd_);
        close(epoll_fd_);
        timer_fd_ = -1;
        epoll_fd_ = -1;
        return;
    }
    
    running_ = true;
    thread_ = std::make_unique<std::thread>([this, callback]() {
        struct epoll_event events[1];
        uint64_t expirations;
        
        while (running_) {
            // 等待定时器事件
            int nfds = epoll_wait(epoll_fd_, events, 1, -1);
            if (nfds == -1) {
                if (errno == EINTR) {
                    continue;  // 被信号中断，重试
                }
                std::cerr << "epoll_wait error: " << strerror(errno) << std::endl;
                break;
            }
            
            if (nfds > 0 && (events[0].events & EPOLLIN)) {
                // 记录实际心跳时间
                auto now = std::chrono::steady_clock::now();
                
                // 读取定时器的到期次数
                ssize_t s = read(timer_fd_, &expirations, sizeof(expirations));
                if (s != sizeof(expirations)) {
                    if (errno == EAGAIN) {
                        continue;  // 非阻塞模式下没有数据可读
                    }
                    std::cerr << "read timerfd error: " << strerror(errno) << std::endl;
                    break;
                }
                
                // 触发心跳回调
                callback(now);
                stats_.recordHeartbeat(now, std::chrono::microseconds(interval_));
            }
        }
    });
}
#else
// TimerfdHeartbeat 在非Linux平台上的模拟实现
TimerfdHeartbeat::TimerfdHeartbeat(std::chrono::milliseconds interval)
    : Heartbeat("Timer模拟(非Linux平台)", interval) {
}

TimerfdHeartbeat::~TimerfdHeartbeat() {
    stop();
}

void TimerfdHeartbeat::start(const HeartbeatCallback& callback) {
    if (running_) {
        return;
    }
    
    running_ = true;
    thread_ = std::make_unique<std::thread>([this, callback]() {
        std::unique_lock<std::mutex> lock(mutex_);
        
        while (running_) {
            // 计算下一个心跳的绝对时间
            auto now = std::chrono::steady_clock::now();
            auto next_time = now + interval_;
            
            // 记录心跳事件
            callback(now);
            stats_.recordHeartbeat(now, std::chrono::microseconds(interval_));
            
            // 等待到下一个心跳时间
            cv_.wait_until(lock, next_time, [this]() { return !running_; });
        }
    });
}
#endif

// SleepHeartbeat 实现
SleepHeartbeat::SleepHeartbeat(std::chrono::milliseconds interval)
    : Heartbeat("Sleep_for", interval) {
}

SleepHeartbeat::~SleepHeartbeat() {
    stop();
}

void SleepHeartbeat::start(const HeartbeatCallback& callback) {
    if (running_) {
        return;
    }
    
    running_ = true;
    thread_ = std::make_unique<std::thread>([this, callback]() {
        while (running_) {
            auto start_time = std::chrono::steady_clock::now();
            
            // 触发心跳回调
            callback(start_time);
            stats_.recordHeartbeat(start_time, std::chrono::microseconds(interval_));
            
            // 简单地睡眠指定的间隔时间
            std::this_thread::sleep_for(interval_);
        }
    });
}