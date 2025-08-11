#include "../../external/glad/include/glad/glad.h"
#include <memory>
#include <string>

class Renderer {
public:
  Renderer();
  ~Renderer();

  bool Initialize();
  void Shutdown();
  void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
  void SetViewport(int x, int y, int width, int height);

  bool IsInitialized() const { return m_initialized; }

private:
  bool m_initialized;
};