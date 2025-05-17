#pragma once
#include "engine/engine.hpp"

class Input{
    public:
    void setWindow(GLFWwindow* window);
    void poll();
    glm::vec2 move();
    bool isJumpPressed();
    bool isJumpDown();
    bool isMenuPressed();
    private:
    GLFWwindow* window = nullptr;
    glm::vec2 moveInternal;
    bool isJump = false;
    bool isJumpLast = false;
    bool isMenu = false;
    bool isMenuLast = false;
};