# DeepSeek API 客户端使用指南

## 项目简介

这个项目提供了一个简单易用的 DeepSeek API 客户端，用于与 DeepSeek 大语言模型进行交互。该客户端支持普通聊天和流式聊天两种模式，可以轻松集成到各种 Python 应用中。

## 功能特点

- 支持普通聊天模式，一次性获取完整回复
- 支持流式聊天模式，实时获取生成内容
- 自动管理对话历史
- 异步接口设计，提高性能和响应速度

## 环境设置与安装

### 创建并激活虚拟环境

在使用之前，强烈建议创建一个虚拟环境来隔离项目依赖：

```bash
# 进入项目目录
cd /Users/bokaichen/user/trae_project/traeDemo/deepseek

# 创建虚拟环境
python3 -m venv venv

# 激活虚拟环境
# 在 macOS/Linux 上:
source venv/bin/activate
# 在 Windows 上:
# venv\Scripts\activate
```

## 安装依赖

在使用之前，请确保安装了必要的依赖：

```bash
# 创建虚拟环境（推荐）
python3 -m venv venv
source venv/bin/activate

# 安装依赖
pip install aiohttp
```

## 使用方法

### 基本用法

```python
import asyncio
from deepseek_api import DeepSeekAPI

async def main():
    # 初始化客户端，传入 API 密钥
    api_key = "your-api-key-here"
    client = DeepSeekAPI(api_key)
    
    # 普通聊天
    response = await client.chat("你好，请介绍一下自己")
    print(response)
    
    # 流式聊天
    async for chunk in client.stream_chat("请用Python写一个简单的Web服务器"):
        print(chunk, end="", flush=True)

if __name__ == "__main__":
    asyncio.run(main())
```

### API 密钥设置

您可以通过两种方式设置 API 密钥：

1. 直接在代码中传入
   ```python
   client = DeepSeekAPI("your-api-key-here")
   ```

2. 通过环境变量设置
   ```bash
   export DEEPSEEK_API_KEY="your-api-key-here"
   ```
   ```python
   client = DeepSeekAPI()  # 将自动从环境变量获取
   ```

### 对话历史管理

客户端会自动管理对话历史，每次对话都会被添加到历史记录中。如果需要清除历史记录：

```python
client.clear_history()
```

## 示例代码

项目中包含了一个完整的示例 `example.py`，展示了如何使用该客户端：

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import asyncio
import os
from deepseek_api import DeepSeekAPI

async def test_chat():
    # 使用 API 密钥初始化客户端
    api_key = "your-api-key-here"
    client = DeepSeekAPI(api_key)
    
    # 测试普通聊天
    response = await client.chat("介绍一下张掖", stream=False)
    print("普通聊天响应:")
    print(response)
    print("\n" + "-"*50 + "\n")
    
    # 测试流式聊天
    print("流式聊天响应:")
    async for chunk in client.stream_chat("请用Python写一个简单的Web服务器"):
        print(chunk, end="", flush=True)
    print("\n")

if __name__ == "__main__":
    asyncio.run(test_chat())
```

## API 参考

### DeepSeekAPI 类

#### 初始化

```python
DeepSeekAPI(api_key: str = None)
```

- `api_key`: DeepSeek API 密钥，如果不提供，将尝试从环境变量 `DEEPSEEK_API_KEY` 获取

#### 方法

- `async chat(message: str, stream: bool = False) -> str`
  
  发送聊天消息并获取完整响应
  - `message`: 用户消息
  - `stream`: 是否使用流式模式（默认为 False）
  - 返回: 助手的回复内容

- `async stream_chat(message: str) -> AsyncGenerator[str, None]`
  
  使用流式方式发送聊天消息并获取响应
  - `message`: 用户消息
  - 返回: 异步生成器，逐步生成助手的回复内容

- `add_message(role: str, content: str) -> None`
  
  添加消息到对话历史
  - `role`: 消息角色（"user" 或 "assistant"）
  - `content`: 消息内容

- `clear_history() -> None`
  
  清除对话历史

## 注意事项

- 请确保您的 API 密钥保密，不要将其硬编码在公开的代码中
- 使用流式聊天时，请确保正确处理异步生成器
- 对于大型应用，建议实现错误重试机制

## 许可证

MIT 许可证