#include "renderer.h"
#include <iostream>

Renderer::Renderer() : m_initialized(false) {}

Renderer::~Renderer() { Shutdown(); }

bool Renderer::Initialize() {
  if (m_initialized) {
    return true;
  }

  if (!gladLoadGL()) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return false;
  }

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_initialized = true;
  return true;
}

void Renderer::Shutdown() { m_initialized = false; }

void Renderer::Clear(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetViewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}
