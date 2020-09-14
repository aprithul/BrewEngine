#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "GlAssert.hpp"
#include <unordered_map>
#include "../Vendor/include/stb_image.h"
#include "Logger.hpp"
#include <vector>
#include <string>

namespace PrEngine
{
    extern int texture_create_status;

    struct TextureData
    {
        stbi_uc* data;
        int width;
        int height;
        int no_of_channels;
    };

    struct Texture
    {

        GLuint id;
        int width;
        int height;
        int no_of_channels;
        stbi_uc* data;

        Texture();
        Texture(const char* path);
        ~Texture();
        virtual void Bind(int slot);
        virtual void Unbind();
    };

    struct TextureCubeMap:public Texture
    {

        
        TextureCubeMap( const std::vector<std::string>& paths);
        ~TextureCubeMap();
        void Bind(int slot) override;
        void Unbind() override;
    };

    void delete_texture_from_library(GLuint id);
    extern std::unordered_map<std::string, TextureData> texture_data_library;
    extern std::unordered_map<std::string, Texture*> texture_library;
}
#endif