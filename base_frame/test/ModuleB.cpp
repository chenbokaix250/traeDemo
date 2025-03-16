#include "ModuleB.h"
#include <iostream>

ModuleB::ModuleB() {}

void ModuleB::registerModule() {
    std::cout << "Registering ModuleB..." << std::endl;
    sendHeartbeat("127.0.0.1", 12345, "ModuleB");
}