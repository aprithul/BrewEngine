#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Entity.hpp"
#include "Engine.hpp"
#include "TimeModule.hpp"
#include "Vector2.hpp"
#include "RendererModule.hpp"

namespace PrEngine
{

    class Camera: public Entity
    {
        public:
            Camera(int width, int height, Renderer2D* renderer2d);
            ~Camera();
            Transform transform;
            int width;
            int height;
            float zoom_amount;
            float aspect_ratio;
            Renderer2D* renderer2d;

            void zoom_in(float zoom_speed);
            void zoom_out(float zoom_speed);
            void start() override;
            void update() override;
            void end() override;

            Vector2<float> get_screen_to_world_position(Vector2<int> screen_pos);
            

        private:
            Vector2<float> speed;

        
    };
}
#endif