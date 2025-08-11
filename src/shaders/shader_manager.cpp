#include "shader_manager.h"
#include <iostream>

ShaderManager::ShaderManager()
    : m_quadVAO(0), m_quadVBO(0), m_initialized(false) {}

ShaderManager::~ShaderManager() { Shutdown(); }

bool ShaderManager::Initialize() {
  if (m_initialized) {
    return true;
  }

  CreateQuadGeometry();

  m_initialized = true;
  return true;
}

void ShaderManager::Shutdown() {
  if (m_quadVAO != 0) {
    glDeleteVertexArrays(1, &m_quadVAO);
    m_quadVAO = 0;
  }
  if (m_quadVBO != 0) {
    glDeleteBuffers(1, &m_quadVBO);
    m_quadVBO = 0;
  }
  m_initialized = false;
}

unsigned int ShaderManager::CreateShader(const std::string &vertexSource,
                                         const std::string &fragmentSource) {
  unsigned int vertexShader =
      CompileShaderFromSource(GL_VERTEX_SHADER, vertexSource);
  if (vertexShader == 0) {
    return 0;
  }

  unsigned int fragmentShader =
      CompileShaderFromSource(GL_FRAGMENT_SHADER, fragmentSource);
  if (fragmentShader == 0) {
    glDeleteShader(vertexShader);
    return 0;
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check for linking errors
  int success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cerr << "Shader program linking failed: " << infoLog << std::endl;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    return 0;
  }

  // Clean up individual shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

void ShaderManager::UseShader(unsigned int shaderProgram) {
  glUseProgram(shaderProgram);
}

void ShaderManager::DeleteShader(unsigned int shaderProgram) {
  if (shaderProgram != 0) {
    glDeleteProgram(shaderProgram);
  }
}

void ShaderManager::SetUniform1f(unsigned int shaderProgram,
                                 const std::string &name, float value) {
  int location = glGetUniformLocation(shaderProgram, name.c_str());
  if (location != -1) {
    glUniform1f(location, value);
  }
}

void ShaderManager::SetUniform2f(unsigned int shaderProgram,
                                 const std::string &name, float x, float y) {
  int location = glGetUniformLocation(shaderProgram, name.c_str());
  if (location != -1) {
    glUniform2f(location, x, y);
  }
}

void ShaderManager::SetUniform3f(unsigned int shaderProgram,
                                 const std::string &name, float x, float y,
                                 float z) {
  int location = glGetUniformLocation(shaderProgram, name.c_str());
  if (location != -1) {
    glUniform3f(location, x, y, z);
  }
}

void ShaderManager::SetUniform4f(unsigned int shaderProgram,
                                 const std::string &name, float x, float y,
                                 float z, float w) {
  int location = glGetUniformLocation(shaderProgram, name.c_str());
  if (location != -1) {
    glUniform4f(location, x, y, z, w);
  }
}

void ShaderManager::SetUniform1i(unsigned int shaderProgram,
                                 const std::string &name, int value) {
  int location = glGetUniformLocation(shaderProgram, name.c_str());
  if (location != -1) {
    glUniform1i(location, value);
  }
}

void ShaderManager::SetUniformMatrix4fv(unsigned int shaderProgram,
                                        const std::string &name,
                                        const float *value) {
  int location = glGetUniformLocation(shaderProgram, name.c_str());
  if (location != -1) {
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
  }
}

void ShaderManager::RenderQuad() {
  if (m_quadVAO == 0) {
    CreateQuadGeometry();
  }

  glBindVertexArray(m_quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void ShaderManager::RenderObjectWithShader(unsigned int shaderProgram,
                                           unsigned int VAO, int vertexCount,
                                           GLenum drawMode) {
  glUseProgram(shaderProgram);

  glBindVertexArray(VAO);
  glDrawArrays(drawMode, 0, vertexCount);
  glBindVertexArray(0);

  glUseProgram(0);
}

// Add method for rendering with element buffer objects (indexed rendering)
void ShaderManager::RenderObjectWithShaderIndexed(unsigned int shaderProgram,
                                                  unsigned int VAO,
                                                  int indexCount,
                                                  GLenum drawMode) {
  glUseProgram(shaderProgram);

  glBindVertexArray(VAO);
  glDrawElements(drawMode, indexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

unsigned int ShaderManager::CompileShaderFromSource(unsigned int type,
                                                    const std::string &source) {
  unsigned int shader = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  // Check compilation status
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::string shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
    std::cerr << shaderType << " shader compilation failed: " << infoLog
              << std::endl;
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

void ShaderManager::CreateQuadGeometry() {
  float quadVertices[] = {// positions        // texture Coords
                          -1.0f, 1.0f, 0.0f, 0.0f,  1.0f, -1.0f, -1.0f, 0.0f,
                          0.0f,  0.0f, 1.0f, -1.0f, 0.0f, 1.0f,  0.0f,

                          -1.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  -1.0f, 0.0f,
                          1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,  1.0f};

  glGenVertexArrays(1, &m_quadVAO);
  glGenBuffers(1, &m_quadVBO);

  glBindVertexArray(m_quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}