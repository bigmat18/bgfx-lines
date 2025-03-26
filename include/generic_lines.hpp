#pragma once
#include "lines_settings.hpp"
#include "lines_utils.hpp"
#include <vector>

namespace lines {

    template<typename Settings>
    class GenericLines {
        Settings mSettings; 
        
    public:
        Settings& settings() { return mSettings; }

        const Settings& settings() const { return mSettings; }

        void swap(GenericLines& other)
        {
            using std::swap;
            swap(mSettings, other.mSettings);
        }

        friend void swap(GenericLines& a, GenericLines& b) { a.swap(b); }

        virtual void draw(uint32_t viewId) const = 0;

        virtual void update(const std::vector<LinesVertex> &points) = 0;

    protected:
        void bindSettingsUniform() const
        {
            mSettings.bindUniform();
        }
    
        static uint64_t drawState()
        {
            return 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                   BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                   UINT64_C(0) | BGFX_STATE_BLEND_ALPHA;
        }
    
        template<typename T>
        static std::pair<T*, bgfx::ReleaseFn> getAllocatedBufferAndReleaseFn(
            uint32_t size)
        {
            T* buffer = new T[size];
    
            return std::make_pair(buffer, [](void* ptr, void*) {
                delete[] static_cast<T*>(ptr);
            });
        }
    };
}