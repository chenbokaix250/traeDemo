#include "BaseFrame.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <map>
#include <boost/asio.hpp>  // 添加Boost.Asio库

using boost::asio::ip::tcp;  // 添加命名空间声明



BaseFrame::BaseFrame() : running(false), timerInterval(std::chrono::milliseconds(1000)), currentLogLevel(INFO) {}

BaseFrame::~BaseFrame() {
    stopService();
}

void BaseFrame::startService() {
    running = true;
    heartbeatThread = std::thread(&BaseFrame::heartbeat, this);
    timerThread = std::thread(&BaseFrame::timer, this);
    log("Service started.");
}

void BaseFrame::stopService() {
    running = false;
    cv.notify_all();
    if (heartbeatThread.joinable()) heartbeatThread.join();
    if (timerThread.joinable()) timerThread.join();
    log("Service stopped.");
}

void BaseFrame::setTimerTask(std::function<void()> task, std::chrono::milliseconds interval) {
    std::lock_guard<std::mutex> lock(mtx);
    timerTask = task;
    timerInterval = interval;
}

void BaseFrame::log(const std::string& message, LogLevel level) {
    if (level < currentLogLevel) return;

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm buf;
    localtime_r(&in_time_t, &buf);

    std::string levelStr;
    switch (level) {
        case DEBUG: levelStr = "DEBUG"; break;
        case INFO: levelStr = "INFO"; break;
        case WARN: levelStr = "WARN"; break;
        case ERROR: levelStr = "ERROR"; break;
    }

    std::ostringstream logStream;
    logStream << "[" << levelStr << "] " << std::put_time(&buf, "%Y-%m-%d %H:%M:%S") << " - " << message;

    std::cout << logStream.str() << std::endl;
    if (logFile.is_open()) {
        logFile << logStream.str() << std::endl;
    }
}

void BaseFrame::sendHeartbeat(const std::string& serverAddress, short port, const std::string& moduleName) {
    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(serverAddress, std::to_string(port));
        boost::asio::connect(socket, endpoints);

        std::string message = "Heartbeat from " + moduleName;
        boost::asio::write(socket, boost::asio::buffer(message));
        log("Heartbeat signal sent to server from module: " + moduleName, INFO);
    } catch (std::exception& e) {
        log(std::string("Failed to send heartbeat: ") + e.what(), ERROR);
    }
}

void BaseFrame::setLogLevel(LogLevel level) {
    currentLogLevel = level;
}

void BaseFrame::loadConfig(const std::string& configFilePath) {
    log("Configuration loaded from " + configFilePath, DEBUG);
}

void BaseFrame::checkHealth() {
    while (running) {
        log("Performing health check...", DEBUG);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void BaseFrame::notifyEvent(const std::string& event) {
    log("Event notification: " + event, INFO);
}

void BaseFrame::heartbeat() {
    while (running) {
        log("Heartbeat signal sent.");
        sendHeartbeat("127.0.0.1", 12345, "ModuleA");  // 发送心跳信号到服务器，添加模块名称
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void BaseFrame::handleException() {
    log("Exception detected, restarting service...");
    stopService();
    startService();
}

void BaseFrame::timer() {
    while (running) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait_for(lock, timerInterval, [this] { return !running; });
        if (timerTask && running) {
            timerTask();
        }
    }
}