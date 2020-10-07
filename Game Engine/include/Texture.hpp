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
        Int_32 width;
        Int_32 height;
        Int_32 no_of_channels;

        void Delete();
    };

    struct Texture
    {
        static std::unordered_map<std::string, TextureData> texture_data_library;
        static std::unordered_map<std::string, Texture*> texture_library;
        static Int_32 texture_create_status;

        static Texture* load_default_texture();
        static Texture* load_texture(const std::string& path);
        static void delete_all_texture_data();
        static void delete_all_textures();

        GLuint id;
        Int_32 width;
        Int_32 height;
        Int_32 no_of_channels;
        stbi_uc* data;
		std::string path;

        virtual void Bind(Int_32 slot);
        virtual void Unbind();

		private:
			Texture(const std::string& path);
			~Texture();
    };

}
#endif
