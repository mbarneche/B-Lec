/**
 * @file main.cpp
 * @brief Application entry point
 */

#include "core/application.h"
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    std::cout << "Starting B-Lec Electricity Simulator..." << std::endl;

    Application app;
    
    if (!app.Initialize()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return 1;
    }

    app.Run();

    return 0;
}
