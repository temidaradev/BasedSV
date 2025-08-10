#include "../external/imgui/backends/imgui_impl_glfw.h"
#include "../external/imgui/backends/imgui_impl_opengl3.h"
#include "../external/imgui/imgui.h"

#include "../external/glad/include/glad/glad.h"
#include "utils/file_utils.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 800, "BasedSV", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  gladLoadGL();
  int fb_width, fb_height;
  glfwGetFramebufferSize(window, &fb_width, &fb_height);
  glViewport(0, 0, fb_width, fb_height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  std::string vertexShaderSource = readFile("assets/shaders/basic.vert");
  std::string fragmentShaderSource = readFile("assets/shaders/basic.frag");
  const char *vertexShaderCStr = vertexShaderSource.c_str();
  const char *fragmentShaderCStr = fragmentShaderSource.c_str();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderCStr, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderCStr, NULL);
  glCompileShader(fragmentShader);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLfloat vertices[] = {
      -0.5f, -0.5f * float(sqrt(3)) / 3,    0.0f, // Lower left corner
      0.5f,  -0.5f * float(sqrt(3)) / 3,    0.0f, // Lower right corner
      0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f  // Upper corner
  };

  GLuint VAO, VBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  bool drawTriangle = true;
  float size = 1.0f;
  float color[4] = {0.8f, 0.3f, 0.02f, 1.0f};

  glUseProgram(shaderProgram);
  glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
  glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1],
              color[2], color[3]);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Editor");
    ImGui::Checkbox("Draw Triangle", &drawTriangle);
    ImGui::SliderFloat("Size", &size, 0.5f, 2.0f);
    ImGui::ColorEdit4("Color", color);
    ImGui::End();

    ImGui::Begin("Triangle View");
    if (drawTriangle) {
      ImDrawList *draw_list = ImGui::GetWindowDrawList();
      ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
      ImVec2 canvas_size = ImGui::GetContentRegionAvail();

      if (canvas_size.x < 100.0f)
        canvas_size.x = 100.0f;
      if (canvas_size.y < 100.0f)
        canvas_size.y = 100.0f;

      draw_list->AddRectFilled(
          canvas_pos,
          ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
          IM_COL32(18, 33, 43, 255));

      ImVec2 center = ImVec2(canvas_pos.x + canvas_size.x * 0.5f,
                             canvas_pos.y + canvas_size.y * 0.5f);
      float triangle_size = size * 100.0f;

      ImVec2 p1 = ImVec2(center.x, center.y - triangle_size);
      ImVec2 p2 = ImVec2(center.x - triangle_size * 0.866f,
                         center.y + triangle_size * 0.5f);
      ImVec2 p3 = ImVec2(center.x + triangle_size * 0.866f,
                         center.y + triangle_size * 0.5f);

      // Convert color to ImU32
      ImU32 triangle_color = IM_COL32(color[0] * 255, color[1] * 255,
                                      color[2] * 255, color[3] * 255);

      // Draw filled triangle
      draw_list->AddTriangleFilled(p1, p2, p3, triangle_color);

      // Make the canvas interactive
      ImGui::InvisibleButton("canvas", canvas_size);
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
