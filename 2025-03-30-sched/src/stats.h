#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <mutex>
#include <iostream>
#include <fstream>

class HeartbeatStats {
public:
    HeartbeatStats(const std::string& name);
    
    // 记录心跳事件
    void recordHeartbeat(std::chrono::steady_clock::time_point actual_time, 
                         std::chrono::microseconds expected_interval);
    
    // 打印统计信息到控制台
    void printStats() const;
    
    // 打印统计信息到文件
    void printStats(std::ostream& out) const;
    
    // 获取统计摘要（用于比较不同实现）
    struct StatsSummary {
        double mean_deviation;
        double std_deviation;
        int64_t max_deviation;
        int64_t p95_deviation;
        int64_t p99_deviation;
        size_t sample_count;
    };
    
    StatsSummary getSummary() const;

private:
    std::string name_;                      // 心跳实现方式名称
    std::vector<int64_t> deviations_;       // 偏差值记录（微秒）
    mutable std::mutex mutex_;              // 保护共享数据的互斥锁
    std::chrono::steady_clock::time_point last_time_; // 上次心跳时间
    bool first_heartbeat_;                  // 是否为第一次心跳
    
    // 计算统计数据
    double calculateMean() const;
    double calculateStdDev(double mean) const;
    int64_t calculatePercentile(double percentile) const;
    
    // 内部打印统计信息方法
    void printStatsInternal(std::ostream& out) const;
};