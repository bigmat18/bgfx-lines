#include <lines.hpp>
#include <triangulated_drawable_lines.hpp>
#include <primitive_drawable_lines.hpp>

#include <cassert>

namespace Lines {

    DrawableLines* create(LinesType type, const std::string name)
    {
        switch (type) {
            case LinesType::TRIANGULATED_LINES: {

                TriangulatedDrawableLines *line = new TriangulatedDrawableLines(
                    "triangulated_lines/vs_triangulated_lines", "triangulated_lines/fs_triangulated_lines", name
                );

                return line;
            }
            case LinesType::PRIMITIVE_LINES: {

                PrimitiveDrawableLines *line = new PrimitiveDrawableLines(
                    "primitive_lines/vs_primitive_lines", "primitive_lines/fs_primitive_lines", name
                );

                return line;
            }
            default:
                assert((void("Lines type is incorrect"), true));
                break;
        }
        assert((void("Lines type is incorrect"), true));
    }

#if 0
    void debugMenu() {

        const bgfx::Stats *stats = bgfx::getStats();
        ImGui::SetNextWindowPos(ImVec2({0, 0}));
        ImGui::SetNextWindowSize(ImVec2({static_cast<float>(stats->width) / 3.8f, static_cast<float>(stats->height)}));

        ImGui::Begin("Lines debugging", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        for(auto& line : s_lines) {
            if(ImGui::CollapsingHeader(line->GetName().c_str())){
                uint16_t vertex_size = line->m_layout.getStride() / ((uint16_t)sizeof(float));

                ImGui::Text("Vertex number: %zu", line->m_vertices.size() / vertex_size);
                ImGui::Text("Index block number: %zu", line->GetState() == UINT64_C(0) ? line->m_indices.size() / 3 : line->m_indices.size() / 2);
                ImGui::Text("VBO size: %lu", line->m_vertices.size() * sizeof(line->m_vertices[0]));
                ImGui::Text("IBO size: %lu", line->m_indices.size() * sizeof(line->m_indices[0]));

                if (ImGui::TreeNode("Vertex buffer")) {
                    std::vector<uint16_t> offsets;
                    for (int i = bgfx::Attrib::Position; i != bgfx::Attrib::Count; i++)
                    {
                        bgfx::Attrib::Enum value = static_cast<bgfx::Attrib::Enum>(i);
                        if (line->m_layout.has(value))
                            offsets.push_back(line->m_layout.getOffset(value) / sizeof(float));
                    }


                    for (uint32_t i = 0; i < line->m_vertices.size(); i += vertex_size)
                    {
                        std::string title = "Vertex " + std::to_string(i / vertex_size);
                        
                        ImGui::SeparatorText(title.c_str());

                        for (uint32_t k = 0; k < offsets.size(); k++)
                        {
                            uint32_t size = k + 1 != offsets.size() ? offsets[k + 1] - offsets[k] : vertex_size - offsets[k];

                            std::stringstream ss;
                            
                            for (uint32_t j = 0; j < size; j++)
                                ss << std::fixed << std::setprecision(3) <<line->m_vertices[i + j + offsets[k]] << " ";

                            std::string element = "" + ss.str();
                            ImGui::BulletText("Layout element %d: %s\n", k, element.c_str());
                        }
                    }
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Index buffer")) {
                    switch (line->GetState()) {
                        case UINT64_C(0): {
                            for(uint32_t i = 0; i < line->m_indices.size(); i+= 3) 
                            {
                                std::string title = "Triangle " + std::to_string(i / 3);
                                ImGui::SeparatorText(title.c_str());
                                ImGui::BulletText("Indices: %d %d %d", line->m_indices[i], line->m_indices[i + 1], line->m_indices[i + 2]);
                            }
                            break;
                        }

                        case BGFX_STATE_PT_LINES: {
                            for (uint32_t i = 0; i < line->m_indices.size(); i += 2)
                            {
                                std::string title = "Line " + std::to_string(i / 2);
                                ImGui::SeparatorText(title.c_str());
                                ImGui::BulletText("Indices: %d %d", line->m_indices[i], line->m_indices[i + 1]);
                            }
                            break;
                        }
                    
                    }
                    ImGui::TreePop();
                }
                ImGui::Checkbox("Render line", &line->m_active);
            }
        }

        ImGui::End();
    }
#endif
}
