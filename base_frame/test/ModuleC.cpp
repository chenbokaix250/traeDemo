#include "ModuleC.h"
#include <iostream>

ModuleC::ModuleC() {}

void ModuleC::registerModule() {
    std::cout << "Registering ModuleC..." << std::endl;
    sendHeartbeat("127.0.0.1", 12345, "ModuleC");
}