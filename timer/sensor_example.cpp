#include "precise_timer.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <iomanip>
#include <ctime>

class SensorDataCollector : public PreciseTimer {
public:
    SensorDataCollector(int sample_interval_ms) 
        : sampleCount_(0), totalJitter_(0) {
        std::cout << "初始化传感器数据采集器，采样间隔: " << sample_interval_ms << "ms" << std::endl;
        
        // 记录开始时间
        startTime_ = std::chrono::steady_clock::now();
        lastSampleTime_ = startTime_;
        
        // 启动定时器
        start(sample_interval_ms);
    }
    
    ~SensorDataCollector() {
        // 计算平均抖动
        double avgJitter = sampleCount_ > 0 ? totalJitter_ / sampleCount_ : 0;
        
        std::cout << "\n传感器数据采集已停止" << std::endl;
        std::cout << "总采样次数: " << sampleCount_ << std::endl;
        std::cout << "平均抖动: " << std::fixed << std::setprecision(2) << avgJitter << "ms" << std::endl;
        
        // 计算平均值和标准差
        if (!temperatures_.empty()) {
            double sum = std::accumulate(temperatures_.begin(), temperatures_.end(), 0.0);
            double mean = sum / temperatures_.size();
            
            double sq_sum = std::accumulate(temperatures_.begin(), temperatures_.end(), 0.0,
                [mean](double acc, double val) { return acc + (val - mean) * (val - mean); });
            double stddev = std::sqrt(sq_sum / temperatures_.size());
            
            std::cout << "温度数据统计:" << std::endl;
            std::cout << "  样本数: " << temperatures_.size() << std::endl;
            std::cout << "  平均值: " << std::fixed << std::setprecision(2) << mean << "°C" << std::endl;
            std::cout << "  标准差: " << std::fixed << std::setprecision(2) << stddev << "°C" << std::endl;
        }
    }
    
protected:
    void onTimer() override {
        // 获取当前时间
        auto now = std::chrono::steady_clock::now();
        
        // 计算距离上次采样的实际时间
        auto actualInterval = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastSampleTime_).count();
        
        // 计算理想间隔和实际间隔的差异（抖动）
        double jitter = std::abs(actualInterval - interval_.count());
        totalJitter_ += jitter;
        
        // 更新上次采样时间
        lastSampleTime_ = now;
        
        // 模拟读取传感器数据
        double temperature = readTemperatureSensor();
        temperatures_.push_back(temperature);
        
        // 打印采样信息
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            now - startTime_).count();
        
        std::cout << "采样 #" << ++sampleCount_ 
                  << " (t+" << elapsed << "s): 温度 = " 
                  << std::fixed << std::setprecision(2) << temperature << "°C"
                  << " (抖动: " << std::fixed << std::setprecision(2) << jitter << "ms)" << std::endl;
        
        // 模拟数据处理时间（不稳定）
        int processingTime = simulateProcessingTime();
        std::this_thread::sleep_for(std::chrono::milliseconds(processingTime));
    }
    
private:
    // 模拟读取温度传感器
    double readTemperatureSensor() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::normal_distribution<> dist(25.0, 2.0); // 均值25°C，标准差2°C
        
        return dist(gen);
    }
    
    // 模拟数据处理时间
    int simulateProcessingTime() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::exponential_distribution<> dist(0.5); // 指数分布
        
        // 生成20-150ms的处理时间
        return 20 + static_cast<int>(dist(gen) * 130);
    }
    
    int sampleCount_;
    double totalJitter_;
    std::vector<double> temperatures_;
    std::chrono::time_point<std::chrono::steady_clock> startTime_;
    std::chrono::time_point<std::chrono::steady_clock> lastSampleTime_;
    std::chrono::milliseconds interval_;
};

int main() {
    std::cout << "精确定时器示例 - 传感器数据采集" << std::endl;
    std::cout << "------------------------------" << std::endl;
    
    // 创建一个每500毫秒采样一次的数据采集器
    SensorDataCollector collector(500);
    
    // 运行20秒后停止
    std::this_thread::sleep_for(std::chrono::seconds(20));
    
    return 0;
}