#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "GlAssert.hpp"
#include "Texture.hpp"
#include "Utils.hpp"
#include "Vector3.hpp"
#include <sstream>
#include <string>
#include <GL/glew.h>

#define MAX_TEXTURES 8


namespace PrEngine
{

	enum class ShaderUniformName
	{
		u_View,
		u_Projection,
		u_sampler2d,
		u_textures,
		u_Dir_Light,
		u_Model,
		u_View_t,
		u_Camera_Position,
		u_Normal_M,
		u_Panning,
		u_Tiling,
		u_Diffuse_Color,
		u_Outline_Color,
		u_Ambient_Strength,
		u_count
	};

    struct Shader	//60 bytes ( 4 + 4*u_count)
    {
		GLuint id;
		GLint uniform_locations[(int)ShaderUniformName::u_count] = {};
		void Delete();


		static std::vector<Shader> shader_library;
		static std::vector<std::string> shader_names;
		static Bool_8 shader_creation_status;
		static Uint_32 load_shader(const std::string& path);
		static inline Shader* get_shader(Uint_32 _shader);
        static Shader* make_shader_program(const std::string& path);
        static void delete_all_shaders();
        static Shader shader_program; // temporary shader storage while creating. pushed to map once created
		static GLuint make_shader(GLenum type,  const std::string& source);
		static void load_uniform_location(const std::string& uniform, const std::string& type);
		static void parse_shader(const std::string& source);
    };

    struct Material //64 bytes
    {
		Vector3<Float_32> diffuse_color;
		Vector2<Float_32> tiling;
		Vector2<Float_32> panning;
		Uint_32 diffuse_textures[MAX_TEXTURES] = {}; // length depends on the total combined textures allowed by opengl
		Uint_32 shader;

		Material();
		Material(Uint_32 shader, Uint_32 texture, const std::string& name);
		~Material();
		void Delete();
		void Bind();
		void Unbind();


        static std::vector<Material> material_library;
		static std::vector<std::string> material_names;
		static Uint_32 load_material(const std::string& name, Bool_8 create_gl_texture, const std::string& name_modifier = "");
		static Bool_8 material_creation_status;
		static inline Material* get_material(Uint_32 mat_id);
        static void delete_all_materials();
        //Material(const std::string& shader_path, const std::vector<std::string>& cubemap_tex_path,const std::string& name);
        //void Generate(const std::string& shader_path, const std::string& diffuse_tex_path, const std::string& name);


    };

	Material* Material::get_material(Uint_32 _material)
	{
		if (_material < material_library.size())
		{
			return &material_library[_material];
		}
		else
			return &material_library[0];
	}

	Shader* Shader::get_shader(Uint_32 _shader)
	{
		if (_shader < shader_library.size())
		{
			return &shader_library[_shader];
		}
		else
			return  &shader_library[0];
	}

} // namespace PrEngine

#endif
