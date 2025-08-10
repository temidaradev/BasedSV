#include "../../external/imgui/imgui.h"

void editorInit(bool &drawTriangle, float &size, float color[4]) {
  ImGui::Begin("Editor");
  ImGui::Checkbox("Draw Triangle", &drawTriangle);
  ImGui::SliderFloat("Size", &size, 0.5f, 2.0f);
  ImGui::ColorEdit4("Color", color);
  ImGui::End();
}