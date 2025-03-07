#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import asyncio
import os
from deepseek_api import DeepSeekAPI

async def test_chat():
    # 直接使用API密钥
    api_key = "sk-3d5fb73d9fa94b3da648ffe1a88bbc63"
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