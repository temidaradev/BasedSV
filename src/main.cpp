#include "../external/imgui/backends/imgui_impl_glfw.h"
#include "../external/imgui/backends/imgui_impl_opengl3.h"
#include "../external/imgui/imgui.h"

#include "../external/glad/include/glad/glad.h"
#include "core/renderer.h"
#include "core/window.h"
#include "shaders/shader_manager.h"
#include "ui/editor.h"
#include "ui/viewport.h"
#include "utils/file_utils.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  Renderer *renderer =
      static_cast<Renderer *>(glfwGetWindowUserPointer(window));
  if (renderer) {
    renderer->SetViewport(0, 0, width, height);
  }
}

int main() {
  Window window(800, 800, "BasedSV");

  Renderer renderer;
  ShaderManager shaderManager;
  if (!renderer.Initialize()) {
    std::cout << "Failed to initialize renderer" << std::endl;
    glfwTerminate();
    return -1;
  }

  window.setFramebufferSizeCallback([&renderer](int width, int height) {
    renderer.SetViewport(0, 0, width, height);
  });

  window.setKeyCallback([](int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwTerminate();

      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
    }
  });

  window.setMouseButtonCallback([](int button, int action, int mods) {

  });

  std::string vertexShaderSource = readFile("assets/shaders/basic.vert");
  std::string fragmentShaderSource = readFile("assets/shaders/basic.frag");

  unsigned int shaderProgram =
      shaderManager.CreateShader(vertexShaderSource, fragmentShaderSource);
  if (shaderProgram == 0) {
    std::cout << "Failed to create shader program" << std::endl;
    glfwTerminate();
    return -1;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window.getGLFWWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  bool drawTriangle = true;
  float size = 1.0f;
  float color[4] = {0.8f, 0.3f, 0.02f, 1.0f};

  glUseProgram(shaderProgram);
  glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
  glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1],
              color[2], color[3]);

  while (!window.shouldClose()) {
    renderer.Clear(0.07f, 0.13f, 0.17f, 1.0f);

    shaderManager.UseShader(shaderProgram);
    shaderManager.SetUniform1f(shaderProgram, "size", size);
    shaderManager.SetUniform4f(shaderProgram, "color", color[0], color[1],
                               color[2], color[3]);
    shaderManager.SetUniform1f(shaderProgram, "u_time", (float)glfwGetTime());

    int width, height;
    window.getSize(width, height);
    shaderManager.SetUniform2f(shaderProgram, "u_resolution", (float)width,
                               (float)height);

    if (drawTriangle) {
      shaderManager.RenderObjectWithShader(
          shaderProgram, shaderManager.m_quadVAO, 6, GL_TRIANGLES);
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    editorInit(drawTriangle, size, color);
    renderViewport(drawTriangle, size, color);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    window.swapBuffers();
    window.pollEvents();
  }

  shaderManager.DeleteShader(shaderProgram);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}
