#ifndef TRANSFORM3D_HPP
#define TRANSFORM3D_HPP

#include "Vector3.hpp"
#include "Logger.hpp"
#include "Component.hpp"
#include "Matrix4x4f.hpp"
#include "Serializable.hpp"
#include <cmath>
#include <string>


#define PI 3.14159265

namespace PrEngine{
    
    class Transform3D : public Component, public Serializable
    {
        public:
            
            void set_position(const Vector3<float>& position);
            void set_position(float x, float y, float z);
            void set_scale(const Vector3<float>& scale);
            void set_scale(float x, float y, float z);
            void set_rotation(const Vector3<float>& rotation);
            void set_rotation(float x, float y, float z);
            
            const Matrix4x4<float>* const get_transformation();
            const Matrix4x4<float>* const get_rotation_transformation();
            const Matrix4x4<float>* const get_translation_transformation();
            const Vector3<float>& get_position();
            const Vector3<float>& get_scale();
            const Vector3<float>& get_rotation();

            const Vector3<float> get_forward();
            const Vector3<float> get_right();
            const Vector3<float> get_up();

            Transform3D();
            Transform3D(Vector3<float> position, Vector3<float> angle, Vector3<float> scale);
            ~Transform3D() override;
            void translate(const Vector3<float>& translation);
            void rotate(float _x, float _y, float _z);
            std::string to_string() override;

            bool dirty;

        private:

            Vector3<float> position;
            Vector3<float> rotation;
            Vector3<float> scale;

            Vector3<float> forward;
            Vector3<float> right;
            Vector3<float> up;

            Matrix4x4<float> translation;
            Matrix4x4<float> scale_m;
            Matrix4x4<float> rotation_x;
            Matrix4x4<float> rotation_y;
            Matrix4x4<float> rotation_z;
            Matrix4x4<float> transformation;
            Matrix4x4<float> rotation_transformation;

            void update_transformation();
            
    };
}
#endif
