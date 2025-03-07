#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import json
import aiohttp
import asyncio
from typing import List, Dict, Optional, AsyncGenerator

class DeepSeekAPI:
    def __init__(self, api_key: str = None):
        """初始化DeepSeek API客户端"""
        self.api_key = api_key or os.getenv('DEEPSEEK_API_KEY')
        if not self.api_key:
            raise ValueError('DeepSeek API密钥未设置')
        
        self.api_base = 'https://api.deepseek.com/v1'
        self.chat_history: List[Dict] = []
    
    async def _make_request(self, endpoint: str, payload: dict) -> dict:
        """发送API请求并处理响应"""
        headers = {
            'Authorization': f'Bearer {self.api_key}',
            'Content-Type': 'application/json'
        }
        
        async with aiohttp.ClientSession() as session:
            try:
                async with session.post(
                    f'{self.api_base}/{endpoint}',
                    headers=headers,
                    json=payload
                ) as response:
                    if response.status != 200:
                        error_text = await response.text()
                        raise Exception(f'API请求失败: {error_text}')
                    return await response.json()
            except Exception as e:
                raise Exception(f'请求处理错误: {str(e)}')
    
    def add_message(self, role: str, content: str) -> None:
        """添加消息到对话历史"""
        self.chat_history.append({
            'role': role,
            'content': content
        })
    
    def clear_history(self) -> None:
        """清除对话历史"""
        self.chat_history.clear()
    
    async def chat(self, message: str, stream: bool = False) -> str:
        """发送聊天消息并获取响应"""
        self.add_message('user', message)
        
        payload = {
            'model': 'deepseek-chat',
            'messages': self.chat_history,
            'stream': stream
        }
        
        try:
            response = await self._make_request('chat/completions', payload)
            assistant_message = response['choices'][0]['message']['content']
            self.add_message('assistant', assistant_message)
            return assistant_message
        except Exception as e:
            print(f'聊天请求失败: {str(e)}')
            return f'错误: {str(e)}'
        
    async def stream_chat(self, message: str) -> AsyncGenerator[str, None]:
        """使用流式方式发送聊天消息并获取响应"""
        self.add_message('user', message)
        
        payload = {
            'model': 'deepseek-chat',
            'messages': self.chat_history,
            'stream': True
        }
        
        headers = {
            'Authorization': f'Bearer {self.api_key}',
            'Content-Type': 'application/json'
        }
        
        session = aiohttp.ClientSession()
        full_response = ""
        try:
            response = await session.post(
                f'{self.api_base}/chat/completions',
                headers=headers,
                json=payload
            )
            
            if response.status != 200:
                error_text = await response.text()
                await session.close()
                raise Exception(f'API请求失败: {error_text}')
            
            async for line in response.content.iter_any():
                if line:
                    line_text = line.decode('utf-8').strip()
                    if line_text.startswith('data:') and line_text != 'data: [DONE]':
                        json_str = line_text[5:].strip()
                        if json_str:
                            try:
                                data = json.loads(json_str)
                                if 'choices' in data and len(data['choices']) > 0:
                                    delta = data['choices'][0].get('delta', {})
                                    if 'content' in delta:
                                        content = delta['content']
                                        full_response += content
                                        yield content
                            except json.JSONDecodeError:
                                pass
            
            # 将完整响应添加到聊天历史
            self.add_message('assistant', full_response)
            
        except Exception as e:
            print(f'流式聊天请求失败: {str(e)}')
            raise
        finally:
            await session.close()