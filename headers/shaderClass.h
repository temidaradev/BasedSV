#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "../glad/include/glad/glad.h"
#include <cerrno>
#include <string>

std::string get_file_contents(const char *filename);

class Shader {
public:
  GLuint ID;
  Shader(const char *vertexFile, const char *fragmentFile);

  void Activate();
  void Delete();

private:
  void CompileErrors(unsigned int shader, const char *type);
};

#endif