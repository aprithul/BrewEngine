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
#include "Transform3D.hpp"

namespace PrEngine
{

    void insertion_sort(std::vector<Sprite*>& arr, Int_32 n);
	//extern RendererOpenGL2D* renderer;

    class SpriteLayer : public RenderLayer
    {
        public:
            SpriteLayer();
            ~SpriteLayer() override;

            void start() override;
            void update() override;
            void end() override;

            std::vector<Sprite*> sprite_list;
        private:
			void UpdateTransforms(Transform3D* transform);
    };
} // namespace Pringin

#endif
