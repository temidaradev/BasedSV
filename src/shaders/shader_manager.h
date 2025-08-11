#include "../../external/glad/include/glad/glad.h"
#include <string>

class ShaderManager {
public:
  ShaderManager();
  ~ShaderManager();
  bool Initialize();
  void Shutdown();

  unsigned int CreateShader(const std::string &vertexSource,
                            const std::string &fragmentSource);
  void UseShader(unsigned int shaderProgram);
  void DeleteShader(unsigned int shaderProgram);

  void SetUniform1f(unsigned int shaderProgram, const std::string &name,
                    float value);
  void SetUniform2f(unsigned int shaderProgram, const std::string &name,
                    float x, float y);
  void SetUniform3f(unsigned int shaderProgram, const std::string &name,
                    float x, float y, float z);
  void SetUniform4f(unsigned int shaderProgram, const std::string &name,
                    float x, float y, float z, float w);
  void SetUniform1i(unsigned int shaderProgram, const std::string &name,
                    int value);
  void SetUniformMatrix4fv(unsigned int shaderProgram, const std::string &name,
                           const float *value);

  void RenderQuad();

  void RenderObjectWithShader(unsigned int shaderProgram, unsigned int VAO,
                              int vertexCount, GLenum drawMode = GL_TRIANGLES);
  void RenderObjectWithShaderIndexed(unsigned int shaderProgram,
                                     unsigned int VAO, int indexCount,
                                     GLenum drawMode = GL_TRIANGLES);

  bool IsInitialized() const { return m_initialized; }

  unsigned int m_quadVAO;
  unsigned int m_quadVBO;

private:
  bool CompileShader(unsigned int shader, const std::string &source);
  unsigned int CompileShaderFromSource(unsigned int type,
                                       const std::string &source);
  void CreateQuadGeometry();

  bool m_initialized;
};