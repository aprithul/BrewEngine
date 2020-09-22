#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "Entity.hpp"
#include "TimeModule.hpp"
#include "Transform3D.hpp"
#include "Graphics.hpp"
#include "Animator.hpp"
#include <string>

namespace PrEngine
{
	class RendererOpenGL2D;

    class Sprite : public Component
    {

        public:
            Sprite(int order);
            ~Sprite();
            std::string to_string() override;
			void start() override;
			void add_to_renderer(RendererOpenGL2D* renderer);

            int order;
            Graphics* graphics;
            Animator* animator;
			std::vector<Texture*> loaded_textures;

    };

}

#endif
