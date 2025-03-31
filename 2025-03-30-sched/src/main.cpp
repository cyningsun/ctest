#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>
#include <csignal>
#include <atomic>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstring>
#include "heartbeat.h"

// 默认参数
constexpr int DEFAULT_TEST_DURATION = 60;  // 运行时间（秒）
constexpr int DEFAULT_HEARTBEAT_INTERVAL = 100;  // 心跳间隔（毫秒）

// 全局变量
std::atomic<bool> g_running(true);
bool g_verbose = false;  // 详细输出模式
bool g_save_report = false;  // 是否保存报告文件
std::string g_output_file = "heartbeat_report.txt";  // 输出文件名

// 信号处理
void signal_handler(int signal) {
    std::cout << "接收到信号: " << signal << ", 准备退出..." << std::endl;
    g_running = false;
}

// 心跳处理回调函数
void heartbeat_callback(const std::chrono::steady_clock::time_point& time) {
    // 这里可以添加实际的心跳处理逻辑
    if (g_verbose) {
        std::cout << "心跳触发: " << std::chrono::steady_clock::now().time_since_epoch().count() << std::endl;
    }
}

// 打印使用帮助
void print_usage(const char* program_name) {
    std::cout << "用法: " << program_name << " [选项] [心跳间隔(毫秒)] [测试时长(秒)]\n\n"
              << "选项:\n"
              << "  -h, --help         显示帮助信息\n"
              << "  -v, --verbose      显示详细输出\n"
              << "  -o, --output FILE  将统计结果保存到文件\n"
              << std::endl;
}

// 将统计结果保存到文件
void save_statistics_to_file(const std::vector<std::unique_ptr<Heartbeat>>& heartbeats,
                             const std::string& filename,
                             int interval_ms,
                             int duration_s) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "无法创建输出文件: " << filename << std::endl;
        return;
    }
    
    file << "=== 心跳测试统计报告 ===\n";
    file << "心跳间隔: " << interval_ms << " 毫秒\n";
    file << "测试时长: " << duration_s << " 秒\n\n";
    
    for (const auto& heartbeat : heartbeats) {
        heartbeat->getStats().printStats(file);
        file << "\n";
    }
    
    file.close();
    std::cout << "统计结果已保存到 " << filename << std::endl;
}

int main(int argc, char* argv[]) {
    // 参数解析
    int heartbeat_interval = DEFAULT_HEARTBEAT_INTERVAL;
    int test_duration = DEFAULT_TEST_DURATION;
    
    // 解析命令行选项
    int arg_index = 1;
    while (arg_index < argc) {
        if (strcmp(argv[arg_index], "-h") == 0 || strcmp(argv[arg_index], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[arg_index], "-v") == 0 || strcmp(argv[arg_index], "--verbose") == 0) {
            g_verbose = true;
            arg_index++;
        } else if (strcmp(argv[arg_index], "-o") == 0 || strcmp(argv[arg_index], "--output") == 0) {
            if (arg_index + 1 < argc) {
                g_save_report = true;
                g_output_file = argv[arg_index + 1];
                arg_index += 2;
            } else {
                std::cerr << "错误: --output 选项需要一个文件名参数" << std::endl;
                return 1;
            }
        } else {
            // 处理位置参数
            break;
        }
    }
    
    // 解析位置参数
    if (arg_index < argc) {
        heartbeat_interval = std::stoi(argv[arg_index++]);
    }
    
    if (arg_index < argc) {
        test_duration = std::stoi(argv[arg_index++]);
    }
    
    // 设置信号处理
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    
    std::cout << "心跳测试开始，将运行 " << test_duration << " 秒" << std::endl;
    std::cout << "心跳间隔设置为 " << heartbeat_interval << " 毫秒" << std::endl;
    
    // 创建三种心跳实现
    std::vector<std::unique_ptr<Heartbeat>> heartbeats;
    
    auto condvar_heartbeat = std::make_unique<CondVarHeartbeat>(std::chrono::milliseconds(heartbeat_interval));
    auto timerfd_heartbeat = std::make_unique<TimerfdHeartbeat>(std::chrono::milliseconds(heartbeat_interval));
    auto sleep_heartbeat = std::make_unique<SleepHeartbeat>(std::chrono::milliseconds(heartbeat_interval));
    
    // 启动所有心跳
    std::cout << "启动心跳线程..." << std::endl;
    condvar_heartbeat->start(heartbeat_callback);
    timerfd_heartbeat->start(heartbeat_callback);
    sleep_heartbeat->start(heartbeat_callback);
    
    heartbeats.push_back(std::move(condvar_heartbeat));
    heartbeats.push_back(std::move(timerfd_heartbeat));
    heartbeats.push_back(std::move(sleep_heartbeat));
    
    // 运行指定的时间
    auto start_time = std::chrono::steady_clock::now();
    
    std::cout << "测试进行中，按 Ctrl+C 提前结束..." << std::endl;
    
    // 显示进度条
    int prev_percent = 0;
    while (g_running) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
        
        if (elapsed >= test_duration) {
            break;
        }
        
        // 更新进度条（每5%更新一次）
        int percent = (elapsed * 100) / test_duration;
        if (percent > prev_percent && percent % 5 == 0) {
            prev_percent = percent;
            std::cout << "\r进度: " << std::setw(3) << percent << "% [";
            int bars = percent / 2;
            for (int i = 0; i < 50; ++i) {
                if (i < bars) std::cout << "=";
                else if (i == bars) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "]" << std::flush;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "\n测试完成，停止所有心跳线程..." << std::endl;
    
    // 停止所有心跳
    for (auto& heartbeat : heartbeats) {
        heartbeat->stop();
    }
    
    // 打印统计信息
    std::cout << "\n统计信息:" << std::endl;
    for (auto& heartbeat : heartbeats) {
        heartbeat->getStats().printStats();
    }
    
    // 如果需要，保存统计结果到文件
    if (g_save_report) {
        save_statistics_to_file(heartbeats, g_output_file, heartbeat_interval, test_duration);
    }
    
    return 0;
}