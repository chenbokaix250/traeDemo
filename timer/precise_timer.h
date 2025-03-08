#ifndef PRECISE_TIMER_H
#define PRECISE_TIMER_H

#include <chrono>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

class PreciseTimer {
public:
    // 构造函数
    PreciseTimer() : running_(false) {}
    
    // 析构函数，确保定时器停止
    virtual ~PreciseTimer() {
        stop();
    }
    
    // 启动定时器，interval为时间间隔（毫秒）
    void start(int interval_ms) {
        if (running_) {
            return;
        }
        
        running_ = true;
        interval_ = std::chrono::milliseconds(interval_ms);
        
        // 启动定时器线程
        timer_thread_ = std::thread([this]() {
            auto next_time = std::chrono::steady_clock::now() + interval_;
            
            while (running_) {
                // 等待到下一次触发时间
                {
                    std::unique_lock<std::mutex> lock(mutex_);
                    cv_.wait_until(lock, next_time, [this]() { return !running_; });
                    
                    if (!running_) {
                        break;
                    }
                }
                
                // 记录任务开始时间
                auto start_time = std::chrono::steady_clock::now();
                
                // 执行任务
                onTimer();
                
                // 计算任务执行时间
                auto end_time = std::chrono::steady_clock::now();
                auto execution_time = end_time - start_time;
                
                // 计算下一次触发时间，补偿执行时间
                next_time += interval_;
                
                // 如果执行时间超过了间隔，则立即触发下一次
                if (next_time <= end_time) {
                    next_time = end_time + std::chrono::milliseconds(1);
                }
            }
        });
    }
    
    // 停止定时器
    void stop() {
        if (!running_) {
            return;
        }
        
        running_ = false;
        
        // 通知等待的线程
        cv_.notify_all();
        
        // 等待线程结束
        if (timer_thread_.joinable()) {
            timer_thread_.join();
        }
    }
    
    // 检查定时器是否运行中
    bool isRunning() const {
        return running_;
    }
    
    // 修改定时器间隔（毫秒）
    void setInterval(int interval_ms) {
        interval_ = std::chrono::milliseconds(interval_ms);
    }
    
protected:
    // 子类需要实现的定时器回调函数
    virtual void onTimer() = 0;
    
private:
    std::atomic<bool> running_;
    std::chrono::milliseconds interval_;
    std::thread timer_thread_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

#endif // PRECISE_TIMER_H