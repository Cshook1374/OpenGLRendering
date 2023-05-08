#include "Input.hpp"
#include <GLFW/glfw3.h>

void Input::processKeyboardInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}