CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror -O2 -pthread

# 根据操作系统设置不同的链接参数
OS := $(shell uname)
ifeq ($(OS),Linux)
    LDFLAGS = -lrt
else
    LDFLAGS = 
endif

# 测试参数
TEST_DURATION = 60
HEARTBEAT_INTERVAL = 100
REPORT_DIR = reports
SUMMARY_REPORT = test_report.md

# 目标文件
TARGET = heartbeat_demo

# 源文件
SRCS = src/main.cpp src/heartbeat.cpp src/stats.cpp

# 目标路径
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin

# 编译生成的目标
$(BIN_DIR)/$(TARGET): $(SRCS)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# 便捷目标
.PHONY: all clean run test-all test-low test-high test-mixed prepare-reports generate-report

all: $(BIN_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(REPORT_DIR)

run: all
	$(BIN_DIR)/$(TARGET)

prepare-reports:
	mkdir -p $(REPORT_DIR)

# 一次性运行所有测试场景
test-all: test-low test-high test-mixed

# 低负载测试
test-low: all prepare-reports
	@echo "运行低负载测试..."
	$(BIN_DIR)/$(TARGET) --output $(REPORT_DIR)/low_load_report.txt $(HEARTBEAT_INTERVAL) $(TEST_DURATION)

# 高负载测试（需要安装stress工具）
test-high: all prepare-reports
	@echo "运行高负载测试..."
	{ stress --cpu 8 & } && \
	$(BIN_DIR)/$(TARGET) --output $(REPORT_DIR)/high_load_report.txt $(HEARTBEAT_INTERVAL) $(TEST_DURATION); \
	status=$$?; \
	kill -TERM `pgrep stress` 2>/dev/null || true; \
	exit $$status

# 混合负载测试（需要安装stress工具）
test-mixed: all prepare-reports
	@echo "运行混合负载测试..."
	{ stress --cpu 4 --io 4 --vm 2 --vm-bytes 128M & } && \
	$(BIN_DIR)/$(TARGET) --output $(REPORT_DIR)/mixed_load_report.txt $(HEARTBEAT_INTERVAL) $(TEST_DURATION); \
	status=$$?; \
	kill -TERM `pgrep stress` 2>/dev/null || true; \
	exit $$status

# 短时测试（用于快速验证）
test-quick: all
	@echo "运行快速测试 (10秒)..."
	$(BIN_DIR)/$(TARGET) $(HEARTBEAT_INTERVAL) 10

# 测试不同心跳间隔
test-interval-10: all prepare-reports
	@echo "测试10毫秒心跳间隔..."
	$(BIN_DIR)/$(TARGET) --output $(REPORT_DIR)/interval_10ms_report.txt 10 $(TEST_DURATION)

test-interval-100: all prepare-reports
	@echo "测试100毫秒心跳间隔..."
	$(BIN_DIR)/$(TARGET) --output $(REPORT_DIR)/interval_100ms_report.txt 100 $(TEST_DURATION)

test-interval-1000: all prepare-reports
	@echo "测试1000毫秒心跳间隔..."
	$(BIN_DIR)/$(TARGET) --output $(REPORT_DIR)/interval_1000ms_report.txt 1000 $(TEST_DURATION)

# 生成综合测试报告
generate-report: prepare-reports
	@echo "正在生成综合测试报告..."
	python3 scripts/generate_report.py --report-dir $(REPORT_DIR) --output $(SUMMARY_REPORT)
	@echo "报告生成完成: $(SUMMARY_REPORT)"

# 完整测试流程：运行所有测试并生成报告
test-complete: test-all generate-report

# Docker 测试目标
.PHONY: docker-build docker-test-low docker-test-high docker-test-mixed

docker-build:
	mkdir -p $(BUILD_DIR)
	docker build -t heartbeat-test -f docker/Dockerfile .

docker-test-low: docker-build prepare-reports
	docker run --rm -v $(PWD)/$(REPORT_DIR):/app/reports --cpus=1 --memory=512m heartbeat-test \
		./heartbeat_demo --output /app/reports/docker_low_load_report.txt $(HEARTBEAT_INTERVAL) $(TEST_DURATION)

docker-test-high: docker-build prepare-reports
	docker run --rm -v $(PWD)/$(REPORT_DIR):/app/reports --cpus=1 --memory=512m heartbeat-test \
		sh -c "stress --cpu 8 & ./heartbeat_demo --output /app/reports/docker_high_load_report.txt $(HEARTBEAT_INTERVAL) $(TEST_DURATION); kill %1"

docker-test-mixed: docker-build prepare-reports
	docker run --rm -v $(PWD)/$(REPORT_DIR):/app/reports --cpus=1 --memory=512m heartbeat-test \
		sh -c "stress --cpu 4 --io 4 --vm 2 --vm-bytes 64M & ./heartbeat_demo --output /app/reports/docker_mixed_load_report.txt $(HEARTBEAT_INTERVAL) $(TEST_DURATION); kill %1"

# 运行Docker中的完整测试并生成报告
docker-test-complete: docker-test-low docker-test-high docker-test-mixed generate-report