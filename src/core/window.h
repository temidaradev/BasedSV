#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <functional>

class Window {
public:
  Window(int width, int height, const char *title);
  ~Window();

  bool shouldClose();
  void swapBuffers();
  void pollEvents();
  GLFWwindow *getGLFWWindow();

  void getSize(int &width, int &height);
  void getMousePos(double &x, double &y);

  void setFramebufferSizeCallback(std::function<void(int, int)> callback);
  void setKeyCallback(std::function<void(int, int, int, int)> callback);
  void setMouseButtonCallback(std::function<void(int, int, int)> callback);

  void setTitle(const std::string &title);
  void setVSync(bool enabled);
  bool isKeyPressed(int key);
  void maximizeWindow();
  void centerWindow();

  double getTime() const { return glfwGetTime(); }

private:
  GLFWwindow *window;
  std::function<void(int, int)> framebufferCallback;
  std::function<void(int, int, int, int)> keyCallback;
  std::function<void(int, int, int)> mouseButtonCallback;

  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
  static void keyCallbackWrapper(GLFWwindow *window, int key, int scancode,
                                 int action, int mods);
  static void mouseButtonCallbackWrapper(GLFWwindow *window, int button,
                                         int action, int mods);
};

#endif