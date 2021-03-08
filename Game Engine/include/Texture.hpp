#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "GlAssert.hpp"
#include <unordered_map>
#include "../Vendor/include/stb_image.h"
#include "Logger.hpp"
#include <vector>
#include <string>

#define MAX_TEXTURE_SIZE 512

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
        static Uint_32 load_texture(const std::string& path, Bool_8 do_create_gl_texture);
        static void delete_all_texture_data();
        static void delete_all_textures();
		static Uint_32 make_array_texture(const std::vector<Uint_32> diffuse_textures);

        GLuint id;
		Int_32 bind_target;
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

		void create_gl_texture();
		Texture(const std::string& path, Int_32 target, Bool_8 do_create_gl_texture);
		Texture(const std::vector<Uint_32> diffuse_textures, Int_32 target);
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
