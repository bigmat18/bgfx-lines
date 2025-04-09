#pragma once
#include "../generic_lines.hpp"

namespace lines
{
    class InstancingBasedLines : public GenericLines<LinesSettings>
    {
        static inline const std::vector<float> VERTICES =
            {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        static inline const std::vector<uint32_t> INDICES = 
            {0, 1, 2, 1, 3, 2};
        
        bgfx::ProgramHandle mLinesPH = LoadProgram(
            "lines/instancing_based_lines/vs_instancing_based_lines",
            "lines/instancing_based_lines/fs_instancing_based_lines");
            
        std::vector<LinesVertex> mPoints;       

        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndexesBH = BGFX_INVALID_HANDLE;
        
        mutable bgfx::InstanceDataBuffer mInstanceDB;

    public:
        InstancingBasedLines(const std::vector<LinesVertex> &points);

        InstancingBasedLines(const InstancingBasedLines &other) = delete;

        InstancingBasedLines(InstancingBasedLines &&other);

        ~InstancingBasedLines();

        InstancingBasedLines &operator=(InstancingBasedLines other);

        void swap(InstancingBasedLines &other);

        friend void swap(InstancingBasedLines& a, InstancingBasedLines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void checkCaps() const
        {
            const bgfx::Caps* caps = bgfx::getCaps();
            const bool instancingSupported =
                bool(caps->supported & BGFX_CAPS_INSTANCING);

            if (!instancingSupported) {
                throw std::runtime_error("Instancing or compute are not supported");
            }
        }

        void generateInstanceDataBuffer() const;

        void allocateVerticesAndIndicesBuffers();

    };
}