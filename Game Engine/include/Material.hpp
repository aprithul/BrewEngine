#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "GlAssert.hpp"
#include "Texture.hpp"
#include "Utils.hpp"
#include <sstream>
#include <string>
#include <GL/glew.h>

namespace PrEngine
{

    struct Shader
    {
        static std::unordered_map<std::string, Shader> shader_library;

        GLuint id;
        std::unordered_map<std::string, std::pair<std::string, GLuint>> uniform_locations;
        void Delete();
		static Shader* load_shader(const std::string& path);
        static Shader* make_shader_program(const std::string& path);
        static void delete_all_shaders();
    	private:
        	static Shader shader_program; // temporary shader storage while creating. pushed to map once created
			static GLuint make_shader(GLenum type,  const std::string& source);
			static void load_uniform_location(const std::string& uniform, const std::string& type);
			static void parse_shader(const std::string& source);
    };

    struct Material
    {
        static std::unordered_map<std::string, Material*> material_library;
		static Material* load_material(const std::string& shader_path, const std::string& diffuse_tex_path, const std::string& name);
		static bool material_creation_status;

        static void delete_all_materials();
        //Material(const std::string& shader_path, const std::vector<std::string>& cubemap_tex_path,const std::string& name);
        //void Generate(const std::string& shader_path, const std::string& diffuse_tex_path, const std::string& name);
        void Delete();
        //std::string source_file_path;
        Texture* diffuse_texture;
        void Bind();
        void Unbind();
        Shader* shader;
        Vector2<float> tiling;
        Vector2<float> panning;

	private:
		Material(const std::string& shader_path, const std::string& diffuse_tex_path, const std::string& name);
		~Material();

    };

} // namespace PrEngine

#endif
