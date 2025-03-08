#include "precise_timer.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <random>

class TimestampPrinter : public PreciseTimer {
public:
    TimestampPrinter(int interval_ms) : count_(0) {
        std::cout << "开始每 " << interval_ms << " 毫秒打印一次时间戳" << std::endl;
        start(interval_ms);
    }
    
    ~TimestampPrinter() {
        std::cout << "时间戳打印器已停止，共打印 " << count_ << " 次" << std::endl;
    }
    
protected:
    void onTimer() override {
        // 获取当前时间
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        // 打印时间戳
        std::cout << "计数: " << ++count_ << " - 时间: " 
                  << std::put_time(std::localtime(&now_time), "%H:%M:%S") 
                  << '.' << std::setfill('0') << std::setw(3) << ms.count() << std::endl;
        
        // 模拟随机工作负载
        int workload = getRandomWorkload();
        std::cout << "  执行工作，耗时: " << workload << "ms" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(workload));
    }
    
private:
    // 生成随机工作负载 (10-200ms)
    int getRandomWorkload() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist(10, 200);
        return dist(gen);
    }
    
    int count_;
};

int main() {
    std::cout << "精确定时器示例 - 时间戳打印器" << std::endl;
    std::cout << "------------------------------" << std::endl;
    
    // 创建一个每1000毫秒(1秒)触发一次的定时器
    TimestampPrinter printer(1000);
    
    // 运行15秒后停止
    std::this_thread::sleep_for(std::chrono::seconds(15));
    
    return 0;
}