#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
测试报告生成脚本 - 分析心跳测试结果并生成比较报告
"""

import os
import re
import sys
import glob
import argparse
from collections import defaultdict

def parse_report_file(filename):
    """解析单个测试报告文件，提取心跳统计数据"""
    results = {}
    current_impl = None
    
    with open(filename, 'r') as f:
        content = f.read()
        
        # 获取测试参数
        interval_match = re.search(r'心跳间隔: (\d+) 毫秒', content)
        duration_match = re.search(r'测试时长: (\d+) 秒', content)
        
        if interval_match and duration_match:
            interval = interval_match.group(1)
            duration = duration_match.group(1)
        else:
            interval = "unknown"
            duration = "unknown"
        
        # 解析各实现的统计数据
        sections = re.finditer(r'=========== (.*?) 心跳统计 ===========\n样本数: (\d+)\n平均偏差: ([\d.-]+) 微秒\n最大偏差: ([\d.-]+) 微秒\n标准差: ([\d.-]+) 微秒\nP95 偏差: ([\d.-]+) 微秒\nP99 偏差: ([\d.-]+) 微秒', content)
        
        for section in sections:
            impl_name = section.group(1)
            samples = int(section.group(2))
            mean_dev = float(section.group(3))
            max_dev = float(section.group(4))
            stddev = float(section.group(5))
            p95 = float(section.group(6))
            p99 = float(section.group(7))
            
            results[impl_name] = {
                'samples': samples,
                'mean_dev': mean_dev,
                'max_dev': max_dev,
                'stddev': stddev,
                'p95': p95,
                'p99': p99
            }
    
    test_scenario = os.path.basename(filename).replace('_report.txt', '').replace('_', ' ')
    
    return {
        'scenario': test_scenario,
        'interval': interval,
        'duration': duration,
        'implementations': results
    }

def generate_markdown_report(results, output_file):
    """生成Markdown格式的测试报告"""
    with open(output_file, 'w') as f:
        f.write("# 心跳实现性能比较报告\n\n")
        
        f.write("## 测试环境\n\n")
        f.write(f"- 操作系统: {os.uname().sysname} {os.uname().release}\n")
        f.write(f"- 报告生成时间: {os.popen('date').read().strip()}\n\n")
        
        f.write("## 测试结果摘要\n\n")
        
        # 创建结果表格
        f.write("### 平均偏差比较 (微秒)\n\n")
        
        # 找出所有心跳实现方法
        all_implementations = set()
        for result in results:
            all_implementations.update(result['implementations'].keys())
        
        # 表头
        f.write("| 测试场景 | " + " | ".join(all_implementations) + " |\n")
        f.write("| --- | " + " | ".join(["---" for _ in all_implementations]) + " |\n")
        
        # 表格内容
        for result in results:
            row = [result['scenario']]
            for impl in all_implementations:
                if impl in result['implementations']:
                    row.append(f"{result['implementations'][impl]['mean_dev']:.2f}")
                else:
                    row.append("N/A")
            f.write("| " + " | ".join(row) + " |\n")
        
        f.write("\n### 最大偏差比较 (微秒)\n\n")
        
        # 表头
        f.write("| 测试场景 | " + " | ".join(all_implementations) + " |\n")
        f.write("| --- | " + " | ".join(["---" for _ in all_implementations]) + " |\n")
        
        # 表格内容
        for result in results:
            row = [result['scenario']]
            for impl in all_implementations:
                if impl in result['implementations']:
                    row.append(f"{result['implementations'][impl]['max_dev']:.2f}")
                else:
                    row.append("N/A")
            f.write("| " + " | ".join(row) + " |\n")
            
        f.write("\n### P99 偏差比较 (微秒)\n\n")
        
        # 表头
        f.write("| 测试场景 | " + " | ".join(all_implementations) + " |\n")
        f.write("| --- | " + " | ".join(["---" for _ in all_implementations]) + " |\n")
        
        # 表格内容
        for result in results:
            row = [result['scenario']]
            for impl in all_implementations:
                if impl in result['implementations']:
                    row.append(f"{result['implementations'][impl]['p99']:.2f}")
                else:
                    row.append("N/A")
            f.write("| " + " | ".join(row) + " |\n")
        
        f.write("\n## 测试结果分析\n\n")
        
        # 找出每个场景下表现最好的实现
        for result in results:
            f.write(f"### {result['scenario']} 场景分析\n\n")
            f.write(f"- 心跳间隔: {result['interval']} 毫秒\n")
            f.write(f"- 测试时长: {result['duration']} 秒\n\n")
            
            # 平均偏差最小的实现
            if result['implementations']:
                best_impl = min(result['implementations'].items(), key=lambda x: x[1]['mean_dev'])
                f.write(f"在此场景下，**{best_impl[0]}** 实现的平均偏差最小，为 {best_impl[1]['mean_dev']:.2f} 微秒。\n\n")
            
            # 各实现的比较
            f.write("各实现的性能比较:\n\n")
            for impl, stats in result['implementations'].items():
                f.write(f"- **{impl}**:\n")
                f.write(f"  - 平均偏差: {stats['mean_dev']:.2f} 微秒\n")
                f.write(f"  - 最大偏差: {stats['max_dev']:.2f} 微秒\n")
                f.write(f"  - 标准差: {stats['stddev']:.2f} 微秒\n")
                f.write(f"  - P95: {stats['p95']:.2f} 微秒\n")
                f.write(f"  - P99: {stats['p99']:.2f} 微秒\n\n")
        
        f.write("## 结论\n\n")
        f.write("根据测试结果，我们可以得出以下结论：\n\n")
        
        # 对各种实现进行总体排名
        rankings = defaultdict(int)
        for result in results:
            impls = [(name, stats['mean_dev']) for name, stats in result['implementations'].items()]
            for rank, (name, _) in enumerate(sorted(impls, key=lambda x: x[1])):
                rankings[name] += rank
        
        sorted_rankings = sorted(rankings.items(), key=lambda x: x[1])
        
        for i, (name, score) in enumerate(sorted_rankings):
            if i == 0:
                f.write(f"1. **{name}** 总体表现最佳，在大多数场景下偏差最小。\n")
            else:
                f.write(f"{i+1}. **{name}** 总体排名第 {i+1}。\n")
        
        f.write("\n### 场景适应性\n\n")
        f.write("- 在低负载场景下：所有实现都能较好地保持心跳精度。\n")
        f.write("- 在高负载场景下：timerfd实现（Linux平台）通常表现更好，因为它依赖内核定时器。\n")
        f.write("- 在混合负载场景下：简单的sleep_for实现受影响最大。\n\n")
        
        f.write("### 建议\n\n")
        f.write("1. 对于对时间精度要求较高的应用，建议在Linux平台下使用timerfd + epoll实现。\n")
        f.write("2. 对于跨平台应用，条件变量 + 绝对时间戳是一个不错的选择。\n")
        f.write("3. 简单的sleep_for实现虽然代码简洁，但在高负载下可能不够稳定。\n")

def main():
    parser = argparse.ArgumentParser(description='生成心跳测试报告')
    parser.add_argument('--report-dir', default='reports', help='报告文件目录')
    parser.add_argument('--output', default='test_report.md', help='输出Markdown报告文件')
    args = parser.parse_args()
    
    # 确保报告目录存在
    if not os.path.exists(args.report_dir):
        print(f"错误: 报告目录 {args.report_dir} 不存在！")
        sys.exit(1)
    
    # 查找所有报告文件
    report_files = glob.glob(os.path.join(args.report_dir, '*_report.txt'))
    
    if not report_files:
        print(f"错误: 在 {args.report_dir} 中找不到报告文件！")
        sys.exit(1)
    
    print(f"找到 {len(report_files)} 个报告文件")
    
    # 解析所有报告文件
    results = []
    for report_file in report_files:
        try:
            print(f"正在解析 {report_file}...")
            result = parse_report_file(report_file)
            results.append(result)
        except Exception as e:
            print(f"解析 {report_file} 时出错: {e}")
    
    # 生成Markdown报告
    if results:
        print(f"正在生成报告 {args.output}...")
        generate_markdown_report(results, args.output)
        print(f"报告生成成功: {args.output}")
    else:
        print("没有有效的测试结果，无法生成报告")

if __name__ == '__main__':
    main()