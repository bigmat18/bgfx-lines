#pragma once
#include "lines_handler.hpp"

namespace Lines {
    enum class LinesType {
        TRIANGULATED_LINES,
        PRIMITIVE_LINES
    };

    class LinesFactory {
        public:
            static LinesHandler *CreateHandler(LinesType type);
            static void Shutdown();
            static void Render(uint64_t state);

        private:
            static std::vector<LinesHandler*> s_lines;
    };
}