#include <polylines/indirect_based_polylines.hpp>

namespace lines {
    IndirectBasedPolylines::IndirectBasedPolylines(const std::vector<Point> &segments, const float width, const float heigth) :
        Polylines(width, heigth, "", "")
    {

    }

    IndirectBasedPolylines::~IndirectBasedPolylines() {

    }


    void IndirectBasedPolylines::draw(uint viewId) const {

    }

    void IndirectBasedPolylines::update(const std::vector<Point> &segments) {

    }

    void IndirectBasedPolylines::allocatePointsBuffer() {

    }

    void IndirectBasedPolylines::generateSegmentsIndirectBuffer() {

    }

    void IndirectBasedPolylines::generateJoinsIndirectBuffer() {

    }
}