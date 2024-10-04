#include <primitive_lines_handler.hpp>

namespace Lines {
    PrimitiveLinesHandler::PrimitiveLinesHandler(uint64_t state, const std::string vs_name, const std::string fs_name) : 
        LinesHandler(state, vs_name, fs_name)
    {

    }

    PrimitiveLinesHandler::~PrimitiveLinesHandler() {}

    void PrimitiveLinesHandler::BeginLine() {}

    void PrimitiveLinesHandler::AddPoint(const LinesPoint &point) {}

    void PrimitiveLinesHandler::EndLine() {}

    void PrimitiveLinesHandler::Render(uint64_t state) {}
}