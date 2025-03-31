#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <string>
#include <unistd.h>

// 系统相关头文件
#ifdef __linux__
#include <sys/epoll.h>
#include <sys/timerfd.h>
#endif

#include "stats.h"

// 心跳回调函数类型定义
using HeartbeatCallback = std::function<void(const std::chrono::steady_clock::time_point&)>;

// 心跳基类
class Heartbeat {
public:
    Heartbeat(const std::string& name, std::chrono::milliseconds interval);
    virtual ~Heartbeat();
    
    // 启动心跳
    virtual void start(const HeartbeatCallback& callback) = 0;
    
    // 停止心跳
    virtual void stop();
    
    // 获取统计信息
    HeartbeatStats& getStats();

protected:
    std::string name_;                // 心跳实现方式名称
    std::chrono::milliseconds interval_; // 心跳间隔
    std::atomic<bool> running_;       // 是否运行中
    std::unique_ptr<std::thread> thread_; // 心跳线程
    HeartbeatStats stats_;            // 统计信息
};

// 基于条件变量和绝对时间戳的心跳实现
class CondVarHeartbeat : public Heartbeat {
public:
    CondVarHeartbeat(std::chrono::milliseconds interval);
    ~CondVarHeartbeat();
    
    void start(const HeartbeatCallback& callback) override;

private:
    std::mutex mutex_;
    std::condition_variable cv_;
};

#ifdef __linux__
// 基于timerfd和epoll的心跳实现（仅Linux支持）
class TimerfdHeartbeat : public Heartbeat {
public:
    TimerfdHeartbeat(std::chrono::milliseconds interval);
    ~TimerfdHeartbeat();
    
    void start(const HeartbeatCallback& callback) override;

private:
    int epoll_fd_;
    int timer_fd_;
};
#else
// 在非Linux平台上的timerfd实现（使用条件变量模拟）
class TimerfdHeartbeat : public Heartbeat {
public:
    TimerfdHeartbeat(std::chrono::milliseconds interval);
    ~TimerfdHeartbeat();
    
    void start(const HeartbeatCallback& callback) override;

private:
    std::mutex mutex_;
    std::condition_variable cv_;
};
#endif

// 基于sleep_for的心跳实现
class SleepHeartbeat : public Heartbeat {
public:
    SleepHeartbeat(std::chrono::milliseconds interval);
    ~SleepHeartbeat();
    
    void start(const HeartbeatCallback& callback) override;
};