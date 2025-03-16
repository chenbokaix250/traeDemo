#ifndef BASE_FRAME_H
#define BASE_FRAME_H

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>
#include <atomic>
#include <boost/asio.hpp>  // 添加Boost.Asio库

enum LogLevel { DEBUG, INFO, WARN, ERROR };

class BaseFrame {
public:
    BaseFrame();
    ~BaseFrame();
    void startService();
    void stopService();
    void setTimerTask(std::function<void()> task, std::chrono::milliseconds interval);
    void setLogLevel(LogLevel level);
    void loadConfig(const std::string& configFilePath);
    void sendHeartbeat(const std::string& serverAddress, short port, const std::string& moduleName);  // 声明函数

private:
    void log(const std::string& message, LogLevel level = INFO);
    void heartbeat();
    void handleException();
    void timer();
    void checkHealth();
    void notifyEvent(const std::string& event);

    std::atomic<bool> running;
    std::thread heartbeatThread;
    std::thread timerThread;
    std::mutex mtx;
    std::condition_variable cv;
    std::function<void()> timerTask;
    std::chrono::milliseconds timerInterval;
    LogLevel currentLogLevel;
    std::ofstream logFile;
};

#endif // BASE_FRAME_H