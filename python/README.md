# MQTT聊天系统

这是一个基于MQTT协议的简单聊天系统，支持多用户实时通信。系统使用Python实现，依赖paho-mqtt库与MQTT代理进行通信。

## 功能特点

- 实时消息传递
- 多房间支持（通过主题区分）
- 简洁的命令行界面
- 时间戳显示
- 用户身份识别

## 系统架构

系统采用发布-订阅模式，主要包含以下组件：

1. **MQTT Broker**：消息中转服务器（如Mosquitto）
2. **发布客户端**：发送消息的聊天客户端
3. **订阅客户端**：接收消息的聊天客户端
4. **消息格式**：使用JSON格式定义消息结构

## 环境要求

- Python 3.6+
- paho-mqtt 2.0+
- Mosquitto MQTT Broker

## 安装步骤

### 1. 安装MQTT Broker

```bash
brew install mosquitto
```

### 2. 创建Python虚拟环境

```bash
python3 -m venv /Users/bokaichen/user/trae_project/traeDemo/venv
source /Users/bokaichen/user/trae_project/traeDemo/venv/bin/activate
```

### 3. 安装依赖

```bash
pip install paho-mqtt
```

## 使用方法

### 1. 启动MQTT Broker

```bash
mosquitto -v
```

### 2. 运行订阅者（接收消息）

```bash
cd /Users/bokaichen/user/trae_project/traeDemo
source venv/bin/activate
python python/subscriber/subscriber.py
```

### 3. 运行发布者（发送消息）

在另一个终端窗口：

```bash
cd /Users/bokaichen/user/trae_project/traeDemo
source venv/bin/activate
python python/publisher/publisher.py
```

## 消息格式

系统使用JSON格式传递消息，包含以下字段：

```json
{
  "sender": "用户名",
  "content": "消息内容",
  "roomId": "房间ID",
  "timestamp": 1234567890
}
```

## 项目结构

```
/Users/bokaichen/user/trae_project/traeDemo/
├── python/
│   ├── publisher/
│   │   └── publisher.py  # 发布者客户端
│   └── subscriber/
│       └── subscriber.py  # 订阅者客户端
├── venv/                  # Python虚拟环境
└── README.md              # 项目说明文档
```

## 扩展功能

本系统可以进一步扩展以支持：

1. 用户认证与授权
2. 消息加密
3. 离线消息存储
4. 图形用户界面
5. 文件传输
6. 多房间管理界面

## 常见问题

### Q: 无法连接到MQTT Broker

确保Mosquitto服务已启动，并检查连接地址和端口是否正确。

### Q: 消息发送成功但接收端没有显示

检查订阅的主题是否与发布的主题匹配。默认情况下，发布者使用"chat/general"主题，订阅者使用"chat/#"订阅所有聊天主题。

### Q: 程序启动时报错"Unsupported callback API version"

确保使用的是paho-mqtt 2.0+版本，并且在创建客户端时指定了正确的callback_api_version参数。

## 许可证

MIT

## 作者

Trae AI-CBK