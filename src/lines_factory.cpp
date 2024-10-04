#include <lines_factory.hpp>
#include <triangulated_lines_handler.hpp>
#include <primitive_lines_handler.hpp>
#include <cassert>

namespace Lines {

    std::vector<LinesHandler *> LinesFactory::s_lines;

    LinesHandler *LinesFactory::CreateHandler(Lines::LinesType type)
    {
        switch (type) {
            case Lines::LinesType::TRIANGULATED_LINES: {
                uint64_t state = 0 | UINT64_C(0);
                TriangulatedLinesHandler *line = new TriangulatedLinesHandler(state, "vs_lines", "fs_lines");
                s_lines.push_back(line);
                return line;
            }
            case Lines::LinesType::PRIMITIVE_LINES: {
                uint64_t state = 0 | BGFX_STATE_PT_LINES;
                PrimitiveLinesHandler *line = new PrimitiveLinesHandler(state, "vs_lines", "fs_lines");
                s_lines.push_back(line);
                return line;
            }
            default:
                assert((void("Lines type is incorrect"), true));
                break;
        }
        return nullptr;
    }

    void LinesFactory::Render(uint64_t state) {
        state &= 0xFFF0FFFFFFFFFFFF;

        for(auto& line : s_lines)
            line->Render(state);
    }

    void LinesFactory::Shutdown() {
        for(auto &line : s_lines)
            delete line;

        s_lines.clear();
    }
}