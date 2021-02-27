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
        static std::vector<Texture> texture_library;
		static std::vector<std::string> texture_names;
		static Uint_32 next_bind_unit;
        static Int_32 texture_create_status;
		static inline Texture* get_texture(Uint_32 texture);
        static Texture* load_default_texture();
        static Uint_32 load_texture(const std::string& path);
        static void delete_all_texture_data();
        static void delete_all_textures();

        GLuint id;
        Int_32 width;
        Int_32 height;
        Int_32 no_of_channels;
        stbi_uc* data;
		Uint_32 path;
		Uint_32 bind_unit;
		//std::string path;

        virtual void Bind(Int_32 slot);
        virtual void Unbind();
		void Delete();


		Texture(const std::string& path);
		~Texture();
    };


	inline Texture* Texture::get_texture(Uint_32 _texture)
	{
		if (_texture < texture_library.size())
		{
			return &texture_library[_texture];
		}
		else
			return &texture_library[0];
	}
}
#endif
