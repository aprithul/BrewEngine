#ifndef EDITOR_MODULE_HPP
#define EDITOR_MODULE_HPP

#include "Module.hpp"
#include "GUI.hpp"
#include "RendererModule.hpp"
#include "Sprite.hpp"
#include "Engine.hpp"
#include "EntityManagementSystemModule.hpp"
#include "Logger.hpp"
#include "Camera.hpp"
#include <string>

namespace PrEngine
{
    class Editor : public Module
    {
        public:
            Editor(std::string name, int priority, Renderer2D* renderer);
            ~Editor();

            void start() override;
            void update() override;
            void end() override;

        private:
            Renderer2D* renderer;
    };

}

#endif