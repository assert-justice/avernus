#include "input.hpp"

void Input::setWindow(GLFWwindow *_window)
{
    window = _window;
}

void Input::poll() {
    float dx = 0; float dy = 0;
    if(glfwGetKey(window, GLFW_KEY_A)) dx -= 1;
    if(glfwGetKey(window, GLFW_KEY_D)) dx += 1;
    if(glfwGetKey(window, GLFW_KEY_W)) dy -= 1;
    if(glfwGetKey(window, GLFW_KEY_S)) dy += 1;
    moveInternal.x = dx;
    moveInternal.y = dy;
    isJumpLast = isJump;
    isJump = glfwGetKey(window, GLFW_KEY_SPACE);
    isMenuLast = isMenu;
    isMenu = glfwGetKey(window, GLFW_KEY_ESCAPE);
}

glm::vec2 Input::move()
{
    return moveInternal;
}

bool Input::isJumpPressed()
{
    return isJump && !isJumpLast;
}

bool Input::isJumpDown()
{
    return isJump;
}

bool Input::isMenuPressed()
{
    return isMenu && !isMenuLast;
}
