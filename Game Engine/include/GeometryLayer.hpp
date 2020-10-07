#ifndef GEOMETRY_LAYER_HPP
#define GEOMETRY_LAYER_HPP

#include "GlAssert.hpp"
#include "RenderLayer.hpp"
#include "Graphics.hpp"
#include "EntityManagementSystemModule.hpp"
#include "DirectionalLight.hpp"
#include "Camera3D.hpp"
#include "Matrix4x4f.hpp"
#include <vector>

namespace PrEngine
{
    class GeometryLayer : public RenderLayer
    {
        public:
            GeometryLayer(long camera_handle);
            ~GeometryLayer() override;

            void start() override;
            void update() override;
            void end() override;

            //std::vector<Graphics*> graphics_list;
            long camera_handle;
    };
} // namespace Pringin

#endif