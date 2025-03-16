#!/bin/bash

# 设置项目根目录
PROJECT_ROOT="/Users/bokaichen/user/trae_project/traeDemo/base_frame"

# 启动服务器
echo "Starting server..."
"$PROJECT_ROOT/server/build/ServerDemo" &

# 启动多个客户端
echo "Starting clients..."
"$PROJECT_ROOT/client/build/ClientDemo" "ModuleA" &
"$PROJECT_ROOT/client/build/ClientDemo" "ModuleB" &
"$PROJECT_ROOT/client/build/ClientDemo" "ModuleC" &

echo "Test started. Press Ctrl+C to stop."
wait