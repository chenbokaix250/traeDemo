#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import time
import datetime
import paho.mqtt.client as mqtt

# MQTT配置
BROKER_ADDRESS = "localhost"
BROKER_PORT = 1883
CLIENT_ID = "python_chat_subscriber"
TOPIC = "chat/#"  # 订阅所有聊天室
QOS = 1

def on_connect(client, userdata, flags, rc, properties=None):
    """连接回调函数"""
    if rc == 0:
        print(f"已连接到MQTT broker: {BROKER_ADDRESS}:{BROKER_PORT}")
        # 订阅主题
        client.subscribe(TOPIC, QOS)
        print(f"已订阅主题: {TOPIC}")
    else:
        print(f"连接失败，返回码: {rc}")

def on_message(client, userdata, msg):
    """消息回调函数"""
    try:
        # 解析JSON消息
        payload = msg.payload.decode('utf-8')
        chat_msg = json.loads(payload)
        
        # 转换时间戳为可读格式
        timestamp = chat_msg.get('timestamp', 0)
        time_str = datetime.datetime.fromtimestamp(timestamp).strftime('%H:%M:%S')
        
        # 显示消息
        print(f"[{time_str}] {chat_msg['sender']} ({chat_msg['roomId']}): {chat_msg['content']}")
    except Exception as e:
        print(f"解析消息错误: {e}")

def main():
    # 创建MQTT客户端，指定回调API版本为2.0
    client = mqtt.Client(client_id=CLIENT_ID, callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
    client.on_connect = on_connect
    client.on_message = on_message
    
    # 连接到broker
    try:
        print(f"正在连接到MQTT broker: {BROKER_ADDRESS}:{BROKER_PORT}")
        client.connect(BROKER_ADDRESS, BROKER_PORT, 60)
    except Exception as e:
        print(f"连接错误: {e}")
        return
    
    # 美化终端输出
    print("\n┌─────────────────────────────────────────┐")
    print("│           MQTT 聊天客户端 - 接收端           │")
    print("├─────────────────────────────────────────┤")
    print("│ 状态: 已连接并等待消息                      │")
    print(f"│ 主题: {TOPIC:<36}│")
    print(f"│ 服务器: {BROKER_ADDRESS:<33}│")
    print("└─────────────────────────────────────────┘\n")
    
    try:
        # 开始网络循环
        client.loop_forever()
    except KeyboardInterrupt:
        print("\n程序被用户中断")
    finally:
        # 断开连接
        client.disconnect()
        print("已断开连接")

if __name__ == "__main__":
    main()