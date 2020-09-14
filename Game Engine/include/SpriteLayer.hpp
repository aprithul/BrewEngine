#ifndef SPRITE_LAYER_HPP
#define SPRITE_LAYER_HPP

#include "GlAssert.hpp"
#include "RenderLayer.hpp"
#include "Graphics.hpp"
#include "EntityManagementSystemModule.hpp"
#include "DirectionalLight.hpp"
#include "Camera3D.hpp"
#include "Matrix4x4f.hpp"
#include <vector>
#include "Sprite.hpp"

namespace PrEngine
{

    void insertion_sort(std::vector<Sprite*>& arr, int n);

    class SpriteLayer : public RenderLayer
    {
        public:
            SpriteLayer(long camera_handle);
            ~SpriteLayer() override;

            void start() override;
            void update() override;
            void end() override;

            std::vector<Sprite*> sprite_list;
            long camera_handle;
    };
} // namespace Pringin

#endif