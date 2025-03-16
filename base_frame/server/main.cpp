#include "HeartbeatServer.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    try {
        boost::asio::io_context io_context;
        HeartbeatServer server(io_context, 12345);

        std::thread serverThread([&io_context]() {
            io_context.run();
        });

        std::this_thread::sleep_for(std::chrono::seconds(30)); // 运行30秒后停止
        server.stop();
        serverThread.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}