#pragma once
#include "lines_handler.hpp"

namespace Lines {
    enum class LinesType {
        TRIANGULATED_LINES,
        PRIMITIVE_LINES
    };

    class LinesFactory {
        public:
            static LinesHandler *CreateHandler(LinesType type, const std::string name = "");
            static void Shutdown();
            static void Render(uint64_t state);

            static void DebugMenu();

        private:
            static std::vector<LinesHandler*> s_lines;
    };
}