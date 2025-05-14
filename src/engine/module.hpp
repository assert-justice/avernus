#pragma once
#include <string>

class Module{
    public:
    void init(bool* hasError);
    void cleanup();
    std::string name = "unnamed module";
    bool isInitalized = false;
    protected:
    virtual bool initInternal() = 0;
    virtual void cleanupInternal() = 0;
    // private:
};