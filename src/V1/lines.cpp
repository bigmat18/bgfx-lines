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
                    "triangulated_polylines/vs_triangulated_lines", "triangulated_polylines/fs_triangulated_lines", name
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
}
