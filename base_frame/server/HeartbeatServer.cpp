#include "HeartbeatServer.h"
#include <boost/asio.hpp>
#include <iostream>
#include <chrono>
#include <unordered_map>  // 添加unordered_map库

HeartbeatServer::HeartbeatServer(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), running(true) {
    startAccept();
    startHeartbeatCheck();
}

void HeartbeatServer::stop() {
    running = false;
}

void HeartbeatServer::startAccept() {
    auto socket = std::make_shared<tcp::socket>(acceptor_.get_executor());
    acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
        if (!ec && running) {
            std::array<char, 128> buffer;
            boost::system::error_code error;
            size_t len = socket->read_some(boost::asio::buffer(buffer), error);
            if (!error) {
                std::string message(buffer.data(), len);
                std::cout << "Heartbeat signal received: " << message << std::endl;
                // 提取模块名称并更新最后心跳时间
                std::string moduleName = message.substr(message.find("from ") + 5);
                lastHeartbeatTimes[moduleName] = std::chrono::steady_clock::now();
            }
            startAccept();
        } else if (running) {
            std::cout << "Failed to receive heartbeat signal: " << ec.message() << std::endl;
        }
    });
}

void HeartbeatServer::startHeartbeatCheck() {
    auto checkInterval = std::chrono::seconds(5);
    auto timer = std::make_shared<boost::asio::steady_timer>(acceptor_.get_executor(), checkInterval);
    timer->async_wait([this, timer](boost::system::error_code ec) {
        if (!ec && running) {
            auto now = std::chrono::steady_clock::now();
            for (const auto& [moduleName, lastTime] : lastHeartbeatTimes) {
                if (now - lastTime > std::chrono::seconds(10)) {
                    std::cout << "No heartbeat signal received from " << moduleName << " for 10 seconds." << std::endl;
                }
            }
            startHeartbeatCheck();
        }
    });
}

// 添加成员变量
std::unordered_map<std::string, std::chrono::steady_clock::time_point> lastHeartbeatTimes;