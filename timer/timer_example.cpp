#include "precise_timer.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>

// 示例：周期性打印当前时间的定时器
class TimeLogger : public PreciseTimer {
public:
    TimeLogger() : count_(0) {}
    
protected:
    void onTimer() override {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        
        // 获取毫秒部分
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::cout << "计数: " << ++count_ << " - 时间: " 
                  << std::put_time(std::localtime(&now_time), "%H:%M:%S") 
                  << '.' << std::setfill('0') << std::setw(3) << ms.count() 
                  << " - 模拟工作时间: " << work_time_ << "ms" << std::endl;
        
        // 模拟不稳定的工作时间
        std::this_thread::sleep_for(std::chrono::milliseconds(work_time_));
        
        // 动态改变工作时间，模拟不稳定的操作
        work_time_ = (work_time_ + 10) % 100;
    }
    
private:
    int count_ = 0;
    int work_time_ = 20; // 初始工作时间20ms
};

int main() {
    std::cout << "启动精确定时器示例，每500ms触发一次" << std::endl;
    
    TimeLogger timer;
    timer.start(500); // 每500ms触发一次
    
    // 运行30秒后停止
    std::this_thread::sleep_for(std::chrono::seconds(30));
    
    timer.stop();
    std::cout << "定时器已停止" << std::endl;
    
    return 0;
}