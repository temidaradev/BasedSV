#include "viewport.h"
#include "../../external/imgui/imgui.h"

void renderViewport(bool drawTriangle, float size, float color[4]) {
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

    ImU32 triangle_color = IM_COL32(color[0] * 255, color[1] * 255,
                                    color[2] * 255, color[3] * 255);

    draw_list->AddTriangleFilled(p1, p2, p3, triangle_color);
    ImGui::InvisibleButton("canvas", canvas_size);
  }
  ImGui::End();
}