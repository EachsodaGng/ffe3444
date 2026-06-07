#include "../rainbow6.hpp"

auto rainbow6::visuals_renderables(bool enabled) -> void 
{

    int ScreenX = GetSystemMetrics(SM_CXSCREEN);
    int ScreenY = GetSystemMetrics(SM_CYSCREEN);

    float Width = static_cast<float>(ScreenX);
    float Height = static_cast<float>(ScreenY);


    for (auto it = cached_entities.begin(); it != cached_entities.end(); ) {
        auto* ent = *it;
        if (!utils::memory::valid_pointer(ent) || !ent->isA()
            || ent->m_Status() != scimitar::entity_status::VALID)
            it = cached_entities.erase(it);
        else
            ++it;
	} //<-- Removes Invalid Entities from the Cache


    if (!enabled)
        return;

    for (auto* Entity : cached_entities) //<-- Start of Entity Loop
    {

        if (!utils::memory::valid_pointer(Entity))
            continue;

        if (!Entity->isA())
            continue;

        if (!(Entity->m_Status() == scimitar::entity_status::VALID))
            continue;

        if (visuals::bSnaplines)
        {

            // Snaplines
            ubiVector2 screen;
            if (scimitar::view_translation::get_instance()->world_to_screen(Entity->m_Origin(), screen))
            {
                ImGui::GetBackgroundDrawList()->AddLine(
                    { Width / 2.0f, Height },
                    { screen.x, screen.y },
                    ImGui::ColorConvertFloat4ToU32(visuals::SnaplineRGB),
                    1.0f
                );
            }


        }

        int distance = math::calculate_distance(scimitar::view_translation::get_instance()->m_ViewOffset(), Entity->m_Origin());


        const float head_distance = 1.72f;


        ubiVector4 bottom = Entity->m_Origin();
        ubiVector4 top = Entity->m_Origin() + ubiVector4(0.0f, 0.0f, head_distance);

        const float half_width = 0.3f;


        ubiVector4 corners[8] = {

            bottom + ubiVector4(-half_width, -half_width, 0),
            bottom + ubiVector4(-half_width,  half_width, 0),
            bottom + ubiVector4(half_width,  half_width, 0),
            bottom + ubiVector4(half_width, -half_width, 0),


            top + ubiVector4(-half_width, -half_width, 0),
            top + ubiVector4(-half_width,  half_width, 0),
            top + ubiVector4(half_width,  half_width, 0),
            top + ubiVector4(half_width, -half_width, 0),
        };


        ubiVector2 screen[8];
        bool valid[8];
        int valid_count = 0;

        for (int i = 0; i < 8; ++i)
        {
            valid[i] = scimitar::view_translation::get_instance()->world_to_screen(corners[i], screen[i]);
            if (valid[i])
                valid_count++;
        }

        if (valid_count == 0)
            continue;


        ImU32 color = ImGui::ColorConvertFloat4ToU32(visuals::BoxRGB);


        if (visuals::bBox) {




            ImU32 color = ImGui::ColorConvertFloat4ToU32(visuals::BoxRGB);

            switch (visuals::iBoxype) {
            case 0: // 3D Box
                for (int i = 0; i < 4; ++i)
                {
                    if (valid[i] && valid[(i + 1) % 4])
                        ImGui::GetBackgroundDrawList()->AddLine(
                            ImVec2(screen[i].x, screen[i].y),
                            ImVec2(screen[(i + 1) % 4].x, screen[(i + 1) % 4].y),
                            color);

                    if (valid[i + 4] && valid[((i + 1) % 4) + 4])
                        ImGui::GetBackgroundDrawList()->AddLine(
                            ImVec2(screen[i + 4].x, screen[i + 4].y),
                            ImVec2(screen[((i + 1) % 4) + 4].x, screen[((i + 1) % 4) + 4].y),
                            color);

                    if (valid[i] && valid[i + 4])
                        ImGui::GetBackgroundDrawList()->AddLine(
                            ImVec2(screen[i].x, screen[i].y),
                            ImVec2(screen[i + 4].x, screen[i + 4].y),
                            color);
                }
                break;

            case 1: // 2D Box
            {

                float left = FLT_MAX, top = FLT_MAX, right = -FLT_MAX, bottom = -FLT_MAX;
                for (int i = 0; i < 8; ++i) {
                    if (!valid[i]) continue;
                    if (screen[i].x < left) left = screen[i].x;
                    if (screen[i].y < top) top = screen[i].y;
                    if (screen[i].x > right) right = screen[i].x;
                    if (screen[i].y > bottom) bottom = screen[i].y;
                }


                ImGui::GetBackgroundDrawList()->AddRect(
                    ImVec2(left, top),
                    ImVec2(right, bottom),
                    color
                );
            }
            break;

            case 2: // 2D Cornered Box
            {
                float left = FLT_MAX, top = FLT_MAX, right = -FLT_MAX, bottom = -FLT_MAX;
                for (int i = 0; i < 8; ++i) {
                    if (!valid[i]) continue;
                    if (screen[i].x < left) left = screen[i].x;
                    if (screen[i].y < top) top = screen[i].y;
                    if (screen[i].x > right) right = screen[i].x;
                    if (screen[i].y > bottom) bottom = screen[i].y;
                }

                float w = right - left;
                float h = bottom - top;
                float line_frac = 0.25f;


                ImGui::GetBackgroundDrawList()->AddLine({ left, top }, { left + w * line_frac, top }, color);
                ImGui::GetBackgroundDrawList()->AddLine({ left, top }, { left, top + h * line_frac }, color);


                ImGui::GetBackgroundDrawList()->AddLine({ right, top }, { right - w * line_frac, top }, color);
                ImGui::GetBackgroundDrawList()->AddLine({ right, top }, { right, top + h * line_frac }, color);


                ImGui::GetBackgroundDrawList()->AddLine({ left, bottom }, { left + w * line_frac, bottom }, color);
                ImGui::GetBackgroundDrawList()->AddLine({ left, bottom }, { left, bottom - h * line_frac }, color);


                ImGui::GetBackgroundDrawList()->AddLine({ right, bottom }, { right - w * line_frac, bottom }, color);
                ImGui::GetBackgroundDrawList()->AddLine({ right, bottom }, { right, bottom - h * line_frac }, color);
            }
            break;
            }
        }
    }
}