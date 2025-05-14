#include "engine.hpp"
#include <iostream>
#include "utils/time.hpp"

bool Engine::hasError()
{
    return errorMessage.length() > 0;
}

void Engine::setError(std::string message)
{
    if(hasError()) return;
    errorMessage = message;
    isRunning = false;
    std::cout << errorMessage << std::endl;
}

void Engine::init()
{
    // if(setup!=nullptr) setup(this);
    // if(hasError()) return;
    if(app == nullptr){
        setError("No app registered!");
        return;
    }
    addModule(&graphics);
    initModules();
    if(hasError()) {
        cleanupModules();
        return;
    }
    app->init();
    loop();
    cleanupModules();
}

double Engine::getTickrate()
{
    return tickRate;
}

void Engine::setTickrate(double tr)
{
    tickRate = tr;
}

double Engine::getDeltaTime()
{
    return dt;
}

double Engine::getFrameTime()
{
    return ft;
}

void Engine::addModule(Module *mod)
{
    modules.push_back(mod);
}

void Engine::initModules()
{
    bool has_err = false;
    for (Module* mod : modules)
    {
        mod->init(&has_err);
        if(has_err) {
            setError("Module \"" + mod->name + "\" failed to initialize.");
            break;
        }
    }
}

void Engine::cleanupModules()
{
    while (modules.size() > 0)
    {
        auto mod = modules.back();
        if(mod->isInitalized) mod->cleanup();
        modules.pop_back();
    }
}

void Engine::loop()
{
    double t = 0.0;
    dt = 1.0 / tickRate; // target delta time
    double currentTime = getTime();
    double acc = 0.0;
    double scriptTime;
    double elapsedTime = 0;
    // if(!vm.launch()) return false;
    // physics.simulate(dt);
    // physics.simulate(dt);
    // physics.simulate(dt);
    while (isRunning){
        double newTime = getTime();
        // time since last frame
        // the frameTime is our budget for game logic
        ft = newTime - currentTime;
        currentTime = newTime;

        // we add the frame time to the accumulator 
        // in this way "left over" time will roll over to the next render loop
        acc += ft;
        // while the time budget has at least one more full time for a script update
        while(isRunning && acc >= dt){
            scriptTime = getTime();
            // input.poll(scriptTime);
            glfwPollEvents();
            // physics.simulate(dt);
            // if(!vm.update()) return false;
            // if(update!=nullptr) update(this);
            app->update();
            elapsedTime = getTime() - scriptTime;
            acc -= dt;
            t += dt;
        }
        graphics.drawBegin();
        // if(!vm.draw()) return false;
        // if(draw!=nullptr) draw(this);
        app->draw();
        graphics.drawUiBegin();
        // if(!vm.drawUi()) return false;
        graphics.drawEnd();
        if(graphics.shouldClose()) isRunning = false;
    }
}
