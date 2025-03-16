#!/bin/bash

# 设置项目根目录
PROJECT_ROOT="/Users/bokaichen/user/trae_project/traeDemo/base_frame"

# 编译客户端
echo "Building client..."
cd "$PROJECT_ROOT/client"
mkdir -p build && cd build
cmake ..
make

# 编译服务器
echo "Building server..."
cd "$PROJECT_ROOT/server"
mkdir -p build && cd build
cmake ..
make

echo "Build completed."