#ifndef RENDER_LAYER_HPP
#define RENDER_LAYER_HPP

#include <string>

namespace PrEngine
{
    class RenderLayer
    {
        public:
            virtual ~RenderLayer();
            virtual void start() = 0;
            virtual void update() = 0;
            virtual void end() = 0;

            std::string name;
    };
}
#endif