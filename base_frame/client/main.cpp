#include "BaseFrame.h"
#include <iostream>
#include <csignal>

// 定义一个全局的BaseFrame指针用于信号处理
BaseFrame* globalBaseFrame = nullptr;

// 信号处理函数
void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    if (globalBaseFrame) {
        globalBaseFrame->stopService();
    }
    exit(signum);
}

void exampleTask() {
    std::cout << "Executing periodic task..." << std::endl;
}

int main() {
    BaseFrame baseFrame;
    globalBaseFrame = &baseFrame;  // 将baseFrame指针赋值给全局变量

    // 注册信号处理函数
    std::signal(SIGINT, signalHandler);

    // 设置定时器任务
    baseFrame.setTimerTask(exampleTask, std::chrono::milliseconds(2000));

    // 启动服务
    baseFrame.startService();

    // 运行一段时间后停止服务
    std::this_thread::sleep_for(std::chrono::seconds(10));
    baseFrame.stopService();

    return 0;
}