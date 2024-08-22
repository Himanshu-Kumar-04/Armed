#include "mainMenuBar.h"
#include <ImGui/imgui.h>

namespace Arm {

    void options(bool& options_flag)
    {
        if (!options_flag)
            return;

        ImGuiStyle& style = ImGui::GetStyle();

        ImGui::SetNextWindowSize(ImVec2(800, 600));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
        ImGui::Begin("Options", (bool*)(&options_flag), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
            | ImGuiWindowFlags_NoResize);

        if (ImGui::BeginMenuBar()) {
            ImGui::Text("Options");
            ImGui::SameLine(ImGui::GetContentRegionAvail().x);
            if (ImGui::Button("Close"))
                options_flag = false;
            ImGui::EndMenuBar();
        }
        ImGui::End();

        ImGui::PopStyleColor();
    }
}