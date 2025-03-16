#ifndef HEARTBEAT_SERVER_H
#define HEARTBEAT_SERVER_H

#include <boost/asio.hpp>
#include <atomic>
#include <chrono>
#include <unordered_map>  // 添加unordered_map库
#include <string>

using boost::asio::ip::tcp;

class HeartbeatServer {
public:
    HeartbeatServer(boost::asio::io_context& io_context, short port);
    void stop();

private:
    void startAccept();
    void startHeartbeatCheck();  // 声明 startHeartbeatCheck 函数

    tcp::acceptor acceptor_;
    std::atomic<bool> running;
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> lastHeartbeatTimes;  // 添加lastHeartbeatTimes变量
};

#endif // HEARTBEAT_SERVER_H