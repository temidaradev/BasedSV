#include "window.h"
#include <iostream>

Window::Window(int width, int height, const char *title) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetKeyCallback(window, keyCallbackWrapper);
  glfwSetMouseButtonCallback(window, mouseButtonCallbackWrapper);
}

Window::~Window() {
  if (window) {
    glfwDestroyWindow(window);
  }
  glfwTerminate();
}

bool Window::shouldClose() { return glfwWindowShouldClose(window); }

void Window::swapBuffers() { glfwSwapBuffers(window); }

void Window::pollEvents() { glfwPollEvents(); }

GLFWwindow *Window::getGLFWWindow() { return window; }

void Window::getSize(int &width, int &height) {
  glfwGetFramebufferSize(window, &width, &height);
}

void Window::getMousePos(double &x, double &y) {
  glfwGetCursorPos(window, &x, &y);
}

void Window::setFramebufferSizeCallback(
    std::function<void(int, int)> callback) {
  framebufferCallback = callback;
}

void Window::setKeyCallback(std::function<void(int, int, int, int)> callback) {
  keyCallback = callback;
}

void Window::setMouseButtonCallback(
    std::function<void(int, int, int)> callback) {
  mouseButtonCallback = callback;
}

void Window::framebufferSizeCallback(GLFWwindow *window, int width,
                                     int height) {
  Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (win && win->framebufferCallback) {
    win->framebufferCallback(width, height);
  }
}

void Window::keyCallbackWrapper(GLFWwindow *window, int key, int scancode,
                                int action, int mods) {
  Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (win && win->keyCallback) {
    win->keyCallback(key, scancode, action, mods);
  }
}

void Window::mouseButtonCallbackWrapper(GLFWwindow *window, int button,
                                        int action, int mods) {
  Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (win && win->mouseButtonCallback) {
    win->mouseButtonCallback(button, action, mods);
  }
}

void Window::setTitle(const std::string &title) {
  if (window) {
    glfwSetWindowTitle(window, title.c_str());
  }
}

void Window::setVSync(bool enabled) { glfwSwapInterval(enabled ? 1 : 0); }

bool Window::isKeyPressed(int key) {
  return glfwGetKey(window, key) == GLFW_PRESS;
}

void Window::maximizeWindow() {
  if (window) {
    glfwMaximizeWindow(window);
  }
}

void Window::centerWindow() {
  if (!window)
    return;

  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  int windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);

  glfwSetWindowPos(window, (mode->width - windowWidth) / 2,
                   (mode->height - windowHeight) / 2);
}