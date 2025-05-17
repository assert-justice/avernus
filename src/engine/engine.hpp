#pragma once
#include <string>
#include <vector>
#include "module.hpp"
#include "graphics.hpp"
#include "app.hpp"

class Engine{
    public:
    // void(*setup)(Engine*) = nullptr;
    // void(*start)(Engine*) = nullptr;
    // void(*update)(Engine*) = nullptr;
    // void(*draw)(Engine*) = nullptr;
    App* app = nullptr;
    bool hasError();
    void setError(std::string message);
    void init();
    double getTickrate();
    void setTickrate(double tr);
    double getDeltaTime();
    double getFrameTime();
    void quit();
    // Modules
    Graphics graphics;
    private:
    std::vector<Module*> modules;
    std::string errorMessage = "";
    double dt;
    double ft;
    double tickRate = 120;
    bool isRunning = true;
    void addModule(Module* mod);
    void initModules();
    void cleanupModules();
    void loop();
};