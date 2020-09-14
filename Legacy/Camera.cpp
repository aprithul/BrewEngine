#include "Camera.hpp"

namespace PrEngine
{
    Camera::Camera(int width, int height, Renderer2D* renderer2d):Entity(ENTITY_TYPE_CAMERA)
    {
        this->width = width;
        this->height = height;
        this->aspect_ratio = width/height;
        this->zoom_amount = 1.0f;
        this->renderer2d = renderer2d;

        add_componenet(&transform);
    }

    Camera::~Camera()
    {

    }

    void Camera::start()
    {
        speed.x = 5.0f;
        speed.y = 5.0f;
    }

    void Camera::update()
    {
        /*if(PrEngine::engine->input_handler->get_key(SDLK_z))
        {
            zoom_amount += zoom_speed;//(zoom_speed * Time::Frame_time);
        }
        else if(PrEngine::engine->input_handler->get_key(SDLK_x))
        {
            zoom_amount -= zoom_speed;//(zoom_speed * Time::Frame_time);
        }

        Vector2<float> move_direction;
        if(engine->input_handler->get_key(SDLK_LEFT))
        {
            move_direction.x = -1;
        }
        else if(engine->input_handler->get_key(SDLK_RIGHT))
        {
            move_direction.x = 1;
        }
        if(engine->input_handler->get_key(SDLK_DOWN))
        {
            move_direction.y = -1;
        }
        else if(engine->input_handler->get_key(SDLK_UP))
        {
            move_direction.y = 1;
        }
       
        transform.translate( (move_direction ^ speed) * Time::Frame_time);
        //LOG(LOGTYPE_GENERAL, std::to_string(transform.position.x).append(", ").append(std::to_string(transform.position.y)));
 */
       

        renderer2d->view_position = transform.position;
        renderer2d->zoom_amount = zoom_amount;

        //LOG(LOGTYPE_GENERAL, std::string("zoom amount: ").append(std::to_string(zoom_amount)));


        SDL_FRect camera_bounds{transform.position.x, transform.position.y, (float)width, (float)height};
        SDL_Color color;
        color.r = 255;
        color.g = 122;
        color.b = 10;
        color.a = 255;
        renderer2d->draw_rectangle(camera_bounds, color);

    }
    void Camera::end()
    {

    }

    void Camera::zoom_in(float zoom_speed)
    {
        zoom_amount += zoom_speed  * Time::Frame_time;
        zoom_amount = PrEngine::clamp(zoom_amount,1.0f, 5.0f);
    }

    void Camera::zoom_out(float zoom_speed)
    {
        zoom_amount -= zoom_speed  * Time::Frame_time;
        zoom_amount = PrEngine::clamp(zoom_amount,1.0f, 5.0f);
    }

    Vector2<float> Camera::get_screen_to_world_position(Vector2<int> screen_pos)
    {
        screen_pos.x -= renderer2d->window_width/2;
        screen_pos.y -= renderer2d->window_height/2;
        screen_pos.y *= -1;
        Vector2<float> ss_to_cs = (screen_pos * (1.f/(float)renderer2d->world_unit_to_pixels/zoom_amount));
        ss_to_cs += transform.position;
        return ss_to_cs;
    }


}