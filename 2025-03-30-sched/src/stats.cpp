#include "stats.h"

HeartbeatStats::HeartbeatStats(const std::string& name)
    : name_(name), first_heartbeat_(true) {
}

void HeartbeatStats::recordHeartbeat(std::chrono::steady_clock::time_point actual_time,
                                     std::chrono::microseconds expected_interval) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (first_heartbeat_) {
        first_heartbeat_ = false;
        last_time_ = actual_time;
        return;
    }
    
    // 计算实际间隔
    auto actual_interval = std::chrono::duration_cast<std::chrono::microseconds>(
        actual_time - last_time_);
    
    // 计算偏差（微秒）
    int64_t deviation = actual_interval.count() - expected_interval.count();
    
    // 存储偏差
    deviations_.push_back(deviation);
    
    // 更新上次心跳时间
    last_time_ = actual_time;
}

void HeartbeatStats::printStats() const {
    printStats(std::cout);
}

void HeartbeatStats::printStats(std::ostream& out) const {
    std::lock_guard<std::mutex> lock(mutex_);
    printStatsInternal(out);
}

void HeartbeatStats::printStatsInternal(std::ostream& out) const {
    if (deviations_.empty()) {
        out << "No heartbeat data available for " << name_ << std::endl;
        return;
    }
    
    double mean = calculateMean();
    double stddev = calculateStdDev(mean);
    int64_t p95 = calculatePercentile(0.95);
    int64_t p99 = calculatePercentile(0.99);
    
    int64_t max_dev = *std::max_element(deviations_.begin(), deviations_.end(), 
        [](int64_t a, int64_t b) { return std::abs(a) < std::abs(b); });
    
    out << "=========== " << name_ << " 心跳统计 ===========" << std::endl;
    out << "样本数: " << deviations_.size() << std::endl;
    out << "平均偏差: " << mean << " 微秒" << std::endl;
    out << "最大偏差: " << max_dev << " 微秒" << std::endl;
    out << "标准差: " << stddev << " 微秒" << std::endl;
    out << "P95 偏差: " << p95 << " 微秒" << std::endl;
    out << "P99 偏差: " << p99 << " 微秒" << std::endl;
    out << "======================================" << std::endl;
}

HeartbeatStats::StatsSummary HeartbeatStats::getSummary() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (deviations_.empty()) {
        return {0.0, 0.0, 0, 0, 0, 0};
    }
    
    double mean = calculateMean();
    
    return {
        mean,
        calculateStdDev(mean),
        *std::max_element(deviations_.begin(), deviations_.end(), 
            [](int64_t a, int64_t b) { return std::abs(a) < std::abs(b); }),
        calculatePercentile(0.95),
        calculatePercentile(0.99),
        deviations_.size()
    };
}

double HeartbeatStats::calculateMean() const {
    if (deviations_.empty()) {
        return 0.0;
    }
    
    int64_t sum = 0;
    for (auto dev : deviations_) {
        sum += dev;
    }
    
    return static_cast<double>(sum) / deviations_.size();
}

double HeartbeatStats::calculateStdDev(double mean) const {
    if (deviations_.empty()) {
        return 0.0;
    }
    
    double sum_sq_diff = 0.0;
    for (auto dev : deviations_) {
        double diff = dev - mean;
        sum_sq_diff += diff * diff;
    }
    
    return std::sqrt(sum_sq_diff / deviations_.size());
}

int64_t HeartbeatStats::calculatePercentile(double percentile) const {
    if (deviations_.empty()) {
        return 0;
    }
    
    std::vector<int64_t> sorted_deviations = deviations_;
    std::sort(sorted_deviations.begin(), sorted_deviations.end());
    
    size_t index = static_cast<size_t>(percentile * (sorted_deviations.size() - 1));
    return sorted_deviations[index];
}