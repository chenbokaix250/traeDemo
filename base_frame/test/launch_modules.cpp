#include <iostream>
#include "ModuleB.h"
#include "ModuleC.h"

int main() {
    ModuleB moduleB;
    ModuleC moduleC;

    std::cout << "Starting ModuleB..." << std::endl;
    moduleB.startService();
    moduleB.registerModule();

    std::cout << "Starting ModuleC..." << std::endl;
    moduleC.startService();
    moduleC.registerModule();

    std::cout << "Modules started. Press Ctrl+C to stop." << std::endl;

    // 保持程序运行，以便可以使用 Ctrl+C 停止所有模块
    while (true) {
        // 可以在这里添加其他逻辑
    }

    return 0;
}