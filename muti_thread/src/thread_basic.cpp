#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;  // 互斥锁用于保护共享数据

void print_message(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);  // RAII方式加锁
    std::cout << "Thread " << std::this_thread::get_id() 
              << ": " << message << std::endl;
}

int main() {
    std::thread t1(print_message, "Hello");
    std::thread t2(print_message, "World");
    
    t1.join();  // 等待线程完成
    t2.join();
    
    return 0;
}