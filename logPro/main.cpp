#include <iostream>
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <atomic>
#include <sstream>
#include <vector>
#include <stdexcept>

template<typename T>
std::string to_string_helper(T&& arg) {
    std::ostringstream oss;
    oss << std::forward<T>(arg);
    return oss.str();
}

class LogQueue {
public:
    void push(const std::string msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(msg);
        cond_var_.notify_one();
    }

    bool pop(std::string& msg) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [this]() {
            return !queue_.empty() || is_shutdowm_;
        });
    
        if (queue_.empty()) {
            return false;
        }
    
        msg = queue_.front();
        queue_.pop();
        return true;
    }

    void shutdown() {
        std::lock_guard<std::mutex> lock(mutex_);
        is_shutdowm_ = true;
        cond_var_.notify_all();
    }

private:
    std::queue<std::string> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
    bool is_shutdowm_ = false;
};

// 修复枚举类定义
enum class LogLevel {
    INFO,
    DEBEG,
    WARN,
    ERROR    // 删除多余的分号
};

// 修复 log 方法
// 删除这段重复的模板函数定义
// template<typename... Args>
// void log(LogLevel level, const std::string& format, Args&&... args) {    // 修改参数为 const 引用
//     std::string level_str;    // 统一变量名为 level_str
//     switch (level) {
//         case LogLevel::INFO:
//             level_str = "[INFO] ";    // 添加空格
//             break;
//         case LogLevel::DEBEG:
//             level_str = "[DEBEG] ";
//             break;
//         case LogLevel::WARN:
//             level_str = "[WARN] ";
//             break;
//         case LogLevel::ERROR:
//             level_str = "[ERROR] ";
//             break;
//     }
//     
//     log_queue_.push(level_str + formatMessage(format, std::forward<Args>(args)...));
// }
class Logger {
public:
    Logger(const std::string& filename) 
        : log_file_(filename, std::ios::out | std::ios::app)
        , exit_flag_(false) {
        if (!log_file_.is_open()) {
            throw std::runtime_error("Failed to open log file");
        }
        worker_thread_ = std::thread([this]() {
            std::string msg;
            while (!exit_flag_) {
                if (log_queue_.pop(msg)) {
                    log_file_ << msg << std::endl;
                    log_file_.flush();  // 确保立即写入文件
                }
            }
        });
    }

    ~Logger() {
        exit_flag_ = true;
        log_queue_.shutdown();
        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }
        if (log_file_.is_open()) {
            log_file_.close();
        }
    }

    template<typename... Args>
    void log(LogLevel level, const std::string& format, Args&&... args) {
        std::string level_str;    // 修改变量名为 level_str
        switch (level) {
            case LogLevel::INFO:
                level_str = "[INFO] ";
                break;
            case LogLevel::DEBEG:
                level_str = "[DEBEG] ";
                break;
            case LogLevel::WARN:
                level_str = "[WARN] ";
                break;
            case LogLevel::ERROR:
                level_str = "[ERROR] ";
                break;
        }
        
        log_queue_.push(level_str + formatMessage(format, std::forward<Args>(args)...));
    }

private:

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        char buffer[80];
        // 使用strftime替代std::put_time来格式化时间
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now_time));
        return std::string(buffer);
    }
    template<typename... Args>
    std::string formatMessage(const std::string& format, Args&&... args) {
        std::vector<std::string> arg_strings = {to_string_helper(std::forward<Args>(args))...};
        std::string result;
        size_t arg_index = 0;
        size_t last_pos = 0;
        size_t pos = 0;
        
        while ((pos = format.find("{}", last_pos)) != std::string::npos) {
            result += format.substr(last_pos, pos - last_pos);
            if (arg_index < arg_strings.size()) {
                result += arg_strings[arg_index];
            }
            arg_index++;
            last_pos = pos + 2;
        }
        result += format.substr(last_pos);

        result = getCurrentTime() + " " + result;
        std::cout << result << std::endl;
        return result;
    }

    LogQueue log_queue_;
    std::thread worker_thread_;
    std::ofstream log_file_;
    std::atomic<bool> exit_flag_;
};

int main() {
    Logger logger("log.txt");
    
    const int num_threads = 5;
    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&logger, i]() {
            for (int j = 0; j < 10; ++j) {
                logger.log(LogLevel::INFO, "Thread {} writing log message {}", i, j);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    return 0;
}