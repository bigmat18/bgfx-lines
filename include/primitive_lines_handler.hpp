#pragma once
#include "lines_handler.hpp"

namespace Lines {

    class PrimitiveLinesHandler : public LinesHandler {
        friend class LinesFactory;

        private:
            PrimitiveLinesHandler(uint64_t state, const std::string vs_name, const std::string fs_name);
            virtual ~PrimitiveLinesHandler();

        public:
            void BeginLine() override;
            void AddPoint(const LinesPoint &point) override;
            void EndLine() override;
            
            void Render(uint64_t state) override;
    };
}