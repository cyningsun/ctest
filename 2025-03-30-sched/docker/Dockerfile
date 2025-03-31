FROM ubuntu:22.04

# 安装必要的依赖
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    stress \
    && rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /app

# 复制源代码
COPY . /app/

# 编译项目
RUN mkdir -p /app/build && \
    cd /app && \
    cmake -B build -S . && \
    cmake --build build

# 复制编译好的可执行文件到根目录
RUN cp build/heartbeat_demo /app/

# 运行命令
CMD ["./heartbeat_demo"]