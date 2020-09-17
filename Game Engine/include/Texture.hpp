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

    struct TextureData
    {
        stbi_uc* data;
        int width;
        int height;
        int no_of_channels;

        void Delete();
    };

    struct Texture
    {
        static std::unordered_map<std::string, TextureData> texture_data_library;
        static std::unordered_map<std::string, Texture*> texture_library;
        static int texture_create_status;

        static void load_default_texture();
        static void delete_all_texture_data();
        static void delete_all_textures();

        GLuint id;
        int width;
        int height;
        int no_of_channels;
        stbi_uc* data;

        Texture();
        Texture(const char* path);
        ~Texture();
        void Delete();
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


}
#endif
