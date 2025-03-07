#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import time
import datetime
import paho.mqtt.client as mqtt

# MQTT配置
BROKER_ADDRESS = "localhost"
BROKER_PORT = 1883
CLIENT_ID = "python_chat_publisher"
QOS = 1

def on_connect(client, userdata, flags, rc, properties=None):
    """连接回调函数"""
    if rc == 0:
        print(f"已连接到MQTT broker: {BROKER_ADDRESS}:{BROKER_PORT}")
    else:
        print(f"连接失败，返回码: {rc}")

def main():
    # 创建MQTT客户端，指定回调API版本为2.0
    client = mqtt.Client(client_id=CLIENT_ID, callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
    client.on_connect = on_connect
    
    # 连接到broker
    try:
        print(f"正在连接到MQTT broker: {BROKER_ADDRESS}:{BROKER_PORT}")
        client.connect(BROKER_ADDRESS, BROKER_PORT, 60)
    except Exception as e:
        print(f"连接错误: {e}")
        return
    
    # 启动网络循环
    client.loop_start()
    
    # 获取用户信息
    username = input("输入您的用户名: ")
    room_id = "general"
    topic = f"chat/{room_id}"
    
    # 美化终端输出
    print("\n┌─────────────────────────────────────────┐")
    print("│           MQTT 聊天客户端 - 发送端           │")
    print("├─────────────────────────────────────────┤")
    print("│ 状态: 已连接并准备发送消息                   │")
    print(f"│ 主题: {topic:<36}│")
    print(f"│ 服务器: {BROKER_ADDRESS:<33}│")
    print("└─────────────────────────────────────────┘\n")
    
    try:
        while True:
            message = input("输入消息 (输入'exit'退出): ")
            
            if message.lower() == 'exit':
                break
            
            # 创建消息
            chat_msg = {
                "sender": username,
                "content": message,
                "roomId": room_id,
                "timestamp": int(time.time())
            }
            
            # 转换为JSON
            payload = json.dumps(chat_msg)
            
            # 发布消息
            result = client.publish(topic, payload, qos=QOS)
            if result.rc != mqtt.MQTT_ERR_SUCCESS:
                print(f"发布失败: {result}")
    except KeyboardInterrupt:
        print("\n程序被用户中断")
    finally:
        # 断开连接
        client.loop_stop()
        client.disconnect()
        print("已断开连接")

if __name__ == "__main__":
    main()