#pragma once 
#include <lines.hpp>

namespace lines {

    class PrimitiveLines : public Lines {

        std::vector<LinesVertex>            mPoints;
        bgfx::VertexBufferHandle            mVerticesBH     = BGFX_INVALID_HANDLE;

        public:
            PrimitiveLines(const std::vector<LinesVertex> &points);

            PrimitiveLines(const PrimitiveLines& other);

            PrimitiveLines(PrimitiveLines&& other);

            ~PrimitiveLines();

            PrimitiveLines& operator=(PrimitiveLines other);

            void swap(PrimitiveLines& other);

            std::shared_ptr<vcl::DrawableObject> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override {};

    };
}