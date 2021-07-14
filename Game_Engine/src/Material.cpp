#include "Material.hpp"
#include <cctype>
#include <assert.h>
namespace PrEngine
{
	//Material empty_material;
	std::vector<Material> Material::material_library;// = { empty_material };
	std::vector<std::string> Material::material_names;// = { "" };

	//Shader empty_shader;
	std::vector<Shader> Shader::shader_library;// = { empty_shader };
	std::vector<std::string> Shader::shader_names;// = { "" };

	Bool_8 Material::material_creation_status;
	Bool_8 Shader::shader_creation_status;

    /*Material::()
    {

    }*/
    /*
    Material::Material(const std::string& shader_path, const std::string& diffuse_tex_path, TextureCubeMap& env_map, const std::string& name)
    {
        // only create new texture on gpu if texture doesn't exist already
        tiling = Vec2f(1,1);
        panning = Vec2f(0,0);
        
        environment_map_texture = &env_map;

        std::unordered_map<std::string, Texture*>::iterator _tex_it = Texture::texture_library.find(diffuse_tex_path);
        if(_tex_it == Texture::texture_library.end()) // texture not in library, so create
        {
            Texture* _tex =  new Texture(diffuse_tex_path.c_str());
            if(Texture::texture_create_status == 0){    // creating texture failed, so assign default
                delete _tex;
                diffuse_texture = Texture::texture_library[get_resource_path("default.jpg")];
            }
            else // successfully created texture, store in library and assign that
            {
                diffuse_texture = _tex;

            }
        }
        else    // texture found in library, so assign that
            diffuse_texture = Texture::texture_library[diffuse_tex_path];

        std::unordered_map<std::string, Shader>::iterator _shader_it = Shader::shader_library.find(shader_path);
        if(_shader_it == Shader::shader_library.end())
        {
            // create shader, probably can be shared, will check later
            //this->source_file_path = std::string(shader_path);
            make_shader_program(this->source_file_path);
            
        }
        else
            shader_program = _shader_it->second;


    }*/

	Material::Material()
	{
		shader = 0;
		std::memset(diffuse_textures, -1, sizeof(Int_32) * 8);
	}

	Material::Material(Uint_32 shader, Uint_32 texture, const std::string& name)
	{
        // only create new texture on gpu if texture doesn't exist already
		//material_creation_status = 1;
        tiling = Vec2f(1,1);
        panning = Vec2f(0,0);
		diffuse_color = Vec3f{ 1,1,1 };

		std::memset(diffuse_textures, -1, sizeof(Int_32) * 8);

		this->shader = shader;
		this->diffuse_textures[0] = texture;
		
		// material id == 0 is invalid 
		// so start with an extra item so we can use material id as index to array
		//material_library.push_back(nullptr);
    }

    /*
    // will create a cubemap texture from the 6 cubemap texture paths
    Material::Material(const std::string& shader_path, const std::vector<std::string>& cubemap_tex_path,const std::string& name)
    {
        // only create new texture on gpu if texture doesn't exist already
        tiling = Vec2f(1,1);
        panning = Vec2f(0,0);
        environment_map_texture = nullptr;

        std::string cubemap_file_name = "";
        for(Int_32 i=0; i<cubemap_tex_path.size(); i++)
        {
            cubemap_file_name += cubemap_tex_path[i];
        }
        std::cout<<"concatanated file name: "<<cubemap_file_name<<std::endl;

        std::unordered_map<std::string, Texture*>::iterator _tex_it = Texture::texture_library.find(cubemap_file_name);
        if(_tex_it == Texture::texture_library.end()) // texture not in library, so create
        {
            Texture* _tex =  new TextureCubeMap(cubemap_tex_path);
            if(Texture::texture_create_status == 0){    // creating texture failed, so assign default
                delete _tex;
                //diffuse_texture = texture_library[get_resource_path("default.jpg")];
            }
            else // successfully created texture, store in library and assign that
            {
                diffuse_texture = _tex;

            }
        }
        else    // texture found in library, so assign that
            diffuse_texture = Texture::texture_library[cubemap_file_name];

        std::unordered_map<std::string, Shader>::iterator _shader_it = Shader::shader_library.find(shader_path);
        if(_shader_it == Shader::shader_library.end())
        {
            // create shader, probably can be shared, will check later
            this->source_file_path = std::string(shader_path);
            make_shader_program(this->source_file_path);

            
            Shader::shader_library[shader_path] = shader_program;
        }
        else
            shader_program = _shader_it->second;

        //load_uniform_location("u_sampler_cube");
        //load_uniform_location("u_MVP");
        

    }
*/

    void Material::Bind()
    {
		auto& _shader = Shader::shader_library[shader].id;
		GL_CALL(
			glUseProgram(_shader))
			for (int i = 0; i < MAX_TEXTURES; i++)
			{
				if (diffuse_textures[i] >= 0)
				{
					Texture* tex = Texture::get_texture(diffuse_textures[i]);
					tex->Bind(tex->bind_unit);
				}
			}
    }

    void Material::Unbind()
    {
        GL_CALL(
            glUseProgram(0))
			for (int i = 0; i < MAX_TEXTURES; i++)
			{
				if (diffuse_textures[i] >= 0)
				{
					Texture::get_texture(diffuse_textures[0])->Unbind();
				}
			}
    }

    void Material::Delete()
    {
		Unbind();
        //GL_CALL(
        //    glDeleteProgram(shader->id))

		// you probably shouldn't delete texture when material is delete, so commenting for now
        /*for(std::unordered_map<std::string, Texture*>::iterator it=texture_library.begin(); it!=texture_library.end(); it++)
        {
            if( it->second!=nullptr)
            {
                if(it->second->id == diffuse_texture->id)
                {
                    delete it->second;
                    it->second = nullptr;
                    LOG(LOGTYPE_GENERAL, "Deleting texture");

                }
            }
        }*/
        
        //delete diffuse_texture;
    }

    Material::~Material()
    {
        
    }

	Uint_32 Material::load_material( const std::string& material_name, Bool_8 do_make_gl_texture, const std::string& name_modifier)
	{
		Int_32 present_at = -1;
		for (int _i=0; _i<material_names.size(); _i++)
		{
			if (material_names[_i] == material_name)
			{
				present_at = _i;
				break;
			}
		}

		Uint_32 material_id = 0;
		//std::unordered_map<Uint_32, Material*>::iterator _mat_it = Material::material_library.find(material_id);
		if (do_make_gl_texture || present_at == -1)
		{
			std::string texture_name = "";
			std::string shader_name = "";

			std::string material_data = read_file(get_resource_path(material_name));
			LOG(LOGTYPE_GENERAL, "Material data:  ", material_data);
			std::stringstream mateiral_data_stream(material_data);
			std::string material_line;
			std::vector<std::string> material_tokens;

			while (std::getline(mateiral_data_stream, material_line))
			{
				material_tokens.clear();
				std::stringstream material_line_stream(material_line);
				std::string material_token;
				while (std::getline(material_line_stream, material_token, ' '))
				{
					material_tokens.push_back(material_token);
				}

				if (material_tokens[0] == "texture")
				{
					texture_name = material_tokens[1];
					trim(texture_name);
				}
				else if (material_tokens[0] == "shader")
				{
					shader_name = material_tokens[1];
					trim(shader_name);
				}
				else
				{
					LOG(LOGTYPE_ERROR, "Couldn't read token : " + material_tokens[0]);
				}

			}

			if (texture_name.empty() || shader_name.empty())
			{
				material_creation_status = 0;
				LOG(LOGTYPE_ERROR, "Material definition incomplete");
				return material_id;
			}

			material_creation_status = 1;
			Uint_32 shader = Shader::load_shader(std::string(shader_name));
			Uint_32 diffuse_texture = Texture::load_texture(texture_name, do_make_gl_texture);
			if (!Texture::texture_create_status)
			{
				material_creation_status = 0;
				LOG(LOGTYPE_ERROR, "Material creation failed, error creating texture");
			}
			else if (!Shader::shader_creation_status)
			{
				material_creation_status = 0;
				LOG(LOGTYPE_ERROR, "Material creation failed, error creating shader");

			}
			else if (present_at == -1)
			{
				Material::material_library.emplace_back(shader, diffuse_texture, material_name);
				Material::material_names.push_back(material_name + name_modifier);
				material_id = material_library.size() - 1;
			}
			else
				material_id = present_at;
		}
		else
			material_id = present_at;
		
		//LOG(LOGTYPE_ERROR, "Material creation failed, error creating texture or shader");

		return material_id;
	}


    void Material::delete_all_materials()
    {
    	/*LOG(LOGTYPE_GENERAL, "Deleting all materials");
		for (int _i = material_library.size()-1; _i>=1; _i--)
        {
			auto& mat = material_library[_i];
			auto& name = material_names[_i];
			//material_library.pop_back();
        	mat.Delete();
        	LOG(LOGTYPE_GENERAL, "Deleted : ", name);
        }*/
		Material::material_library.clear();
		Material::material_names.clear();


	}

    Shader Shader::shader_program;
    void Shader::delete_all_shaders()
    {
    	/*LOG(LOGTYPE_GENERAL, "Deleting all shaders");
        for(auto& it: shader_library)
        {
			it.uniform_locations.clear();
        	GL_CALL(
        			glDeleteProgram(it.id))
			LOG(LOGTYPE_GENERAL, "Deleted : ", std::to_string(it.id ));
        }*/
		Shader::shader_library.clear();
		Shader::shader_names.clear();
	}
    void Shader::load_uniform_location(const std::string& uniform, const std::string& type)
    {
        GLint loc = -1;
        GL_CALL(
            loc = glGetUniformLocation(shader_program.id, uniform.c_str()))
        
        //std::cout<<"Location: "<<uniform<<" , "<<loc<<std::endl;
        //if(loc != -1)

		ShaderUniformName _uniform;
		if (uniform == "u_View")
			_uniform = ShaderUniformName::u_View;
		else if (uniform == "u_Projection")
			_uniform = ShaderUniformName::u_Projection;
		else if (uniform == "u_sampler2d")
			_uniform = ShaderUniformName::u_sampler2d;
		else if (uniform == "u_sampler2darr[0]")
			_uniform = ShaderUniformName::u_sampler2darr0;
		else if (uniform == "u_sampler2darr[1]")
			_uniform = ShaderUniformName::u_sampler2darr1;
		else if (uniform == "u_sampler2darr[2]")
			_uniform = ShaderUniformName::u_sampler2darr2;
		else if (uniform == "u_sampler2darr[3]")
			_uniform = ShaderUniformName::u_sampler2darr3;
		else if (uniform == "u_sampler2darr[4]")
			_uniform = ShaderUniformName::u_sampler2darr4;
		else if (uniform == "u_sampler2darr[5]")
			_uniform = ShaderUniformName::u_sampler2darr5;
		else if (uniform == "u_sampler2darr[6]")
			_uniform = ShaderUniformName::u_sampler2darr6;
		else if (uniform == "u_sampler2darr[7]")
			_uniform = ShaderUniformName::u_sampler2darr7;
		else if (uniform == "u_textures")
			_uniform = ShaderUniformName::u_textures;
		else if (uniform == "u_Dir_Light")
			_uniform = ShaderUniformName::u_Dir_Light;
		else if (uniform == "u_Model")
			_uniform = ShaderUniformName::u_Model;
		else if (uniform == "u_View_t")
			_uniform = ShaderUniformName::u_View_t;
		else if (uniform == "u_Camera_Position")
			_uniform = ShaderUniformName::u_Camera_Position;
		else if (uniform == "u_Normal_M")
			_uniform = ShaderUniformName::u_Normal_M;
		else if (uniform == "u_Panning")
			_uniform = ShaderUniformName::u_Panning;
		else if (uniform == "u_Tiling")
			_uniform = ShaderUniformName::u_Tiling;
		else if (uniform == "u_Diffuse_Color")
			_uniform = ShaderUniformName::u_Diffuse_Color;
		else if (uniform == "u_Outline_Color")
			_uniform = ShaderUniformName::u_Outline_Color;
		else if (uniform == "u_Ambient_Strength")
			_uniform = ShaderUniformName::u_Ambient_Strength;
		else
			LOG(LOGTYPE_ERROR, "Shader uniform unknown ", std::to_string((Int_32)_uniform));
        
		shader_program.uniform_locations[(int)_uniform] = loc;
    }

    void Shader::parse_shader(const std::string& source)
     {
		//LOG(LOGTYPE_GENERAL, source);
        Int_32 pos = 0;
		for (Uint_32 _i = 0; _i < (Int_32)ShaderUniformName::u_count; _i++)
			shader_program.uniform_locations[_i] = -1;

        while((pos = source.find("uniform",pos)) != std::string::npos)
        {
            Int_32 prev = 1;
            while(pos-prev>=0 && source[pos-prev]==' ')
                prev++;
            if(pos-prev<0 || source[pos-prev] !='/') // ignore comments, ignores spaces
            {
                std::string u_type = ""; 
                std::string u_name = "";
                
                Int_32 start = pos+8; // "uniform" length is 7
                Int_32 i = 0;
                for(; source[start+i]!=' '; i++);
                
                u_type = source.substr(start,i);

                //skip through spaces
                start = start+i;
                while(source[start]==' ')
                    start++;
                
                i = 0;
                for(; !std::isspace(source[start+i]) && source[start+i]!=';'; i++);
                u_name = source.substr(start, i);
				std::size_t pos = u_name.find_first_of('['); // is the uniform an array
				if (pos != -1)
				{
					u_name = u_name.substr(0, pos)+"[0]";
					load_uniform_location(u_name, u_type);
					u_name = u_name.substr(0, pos) + "[1]";
					load_uniform_location(u_name, u_type);
					u_name = u_name.substr(0, pos) + "[2]";
					load_uniform_location(u_name, u_type);
					u_name = u_name.substr(0, pos) + "[3]";
					load_uniform_location(u_name, u_type);
					u_name = u_name.substr(0, pos) + "[4]";
					load_uniform_location(u_name, u_type);
					u_name = u_name.substr(0, pos) + "[5]";
					load_uniform_location(u_name, u_type);
					u_name = u_name.substr(0, pos) + "[6]";
					load_uniform_location(u_name, u_type);
					u_name = u_name.substr(0, pos) + "[7]";
					load_uniform_location(u_name, u_type);

				}
				else
	                load_uniform_location(u_name, u_type);
            }



            pos++;
        }
    }



	Uint_32 Shader::load_shader(const std::string& path)
	{
		//Uint_32 shader_id = str_hash(path);
		Int_32 present_at = -1;
		for (int _i = 0; _i < shader_names.size(); _i++)
		{
			if (shader_names[_i] == path)
			{
				present_at = _i;
				break;
			}
		}

		Uint_32 shader_id = 0;
		shader_creation_status = 1;
		if (present_at == -1)
		{
			// create shader, probably can be shared, will check later
			//this->source_file_path = std::string(shader_path);

			if (Shader::make_shader_program(path) == nullptr)
			{
				LOG(LOGTYPE_ERROR, "Shader making failed : ", path);
				shader_creation_status = 0;
				shader_id = 0;
			}
			else
				shader_id = shader_library.size() - 1;
		}
		else
			shader_id = present_at;

		LOG(LOGTYPE_GENERAL, "Shader making successful : ", path);

		return shader_id;
	}

    Shader* Shader::make_shader_program(const std::string& path)
    {
		//Uint_32 _hash = str_hash(path);
		std::string resource_path = get_resource_path(path);
        std::string _source = read_file(resource_path);
        std::stringstream shader_source;
        shader_source << _source;
        
        std::string vert = "";
        std::string frag = "";
        std::string line = "";
        GLenum type = GL_VERTEX_SHADER;
        while(std::getline(shader_source, line))
        {
            //std::cout<<line<<std::endl;
            if(line.find("#vertex") != std::string::npos){
                type = GL_VERTEX_SHADER;

            }
            else if(line.find("#fragment")!= std::string::npos){
                type = GL_FRAGMENT_SHADER;
            }
            else
            {
                if(type == GL_VERTEX_SHADER)
                    vert += line+"\n";
                else if(type == GL_FRAGMENT_SHADER)
                    frag += line+"\n";
            }
        }

        shader_program.id = glCreateProgram();
		for (Uint_32 _i = 0; _i < (int)ShaderUniformName::u_count; _i++)
			shader_program.uniform_locations[_i] = -1;

        if(shader_program.id == 0)
        {
            std::cout<<"Couldn't create shader program"<<std::endl;
            return nullptr;
        }

        GLuint v = make_shader( GL_VERTEX_SHADER, vert);
        GLuint f = make_shader( GL_FRAGMENT_SHADER, frag);
        if(v != -1 && f != -1)
        {
            glAttachShader(shader_program.id, v);
            glAttachShader(shader_program.id, f);
            glLinkProgram(shader_program.id);
            GLint program_linked;

            glGetProgramiv(shader_program.id, GL_LINK_STATUS, &program_linked);
            if (program_linked != GL_TRUE)
            {
                Int_32 length;
                glGetProgramiv(shader_program.id, GL_INFO_LOG_LENGTH, &length);
                GLchar* log = (GLchar*)alloca(length*sizeof(GLchar));
                glGetProgramInfoLog(shader_program.id, length, &length, log);
                LOG(LOGTYPE_ERROR, "Shader linking error\n",std::string(log));
                
                glDetachShader(shader_program.id, v);
                glDeleteShader(v);
                glDetachShader(shader_program.id, f);
                glDeleteShader(f);       
            }
            else
            {   
                glDetachShader(shader_program.id, v);
                glDeleteShader(v);
                glDetachShader(shader_program.id, f);
                glDeleteShader(f);
                parse_shader(_source);

                Shader::shader_library.push_back(shader_program);
				Shader::shader_names.push_back(path);
                return &(Shader::shader_library.back());
            }
        }
        return nullptr;
    }

    GLuint Shader::make_shader( GLenum type,  const std::string& source)
    {
        GLuint shader =  glCreateShader(type);
        if(shader == 0)
            std::cout<<"Couldn't create shader"<<std::endl;
            
        const Char_8* _source = source.c_str();
        glShaderSource(shader,1, &(_source) , NULL);
        glCompileShader(shader);
        
        // check
        GLint compilation_status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_status);
        if(compilation_status != GL_TRUE)
        {
            Int_32 length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            GLchar* log = (GLchar*)alloca(length*sizeof(GLchar));
            glGetShaderInfoLog(shader, length, &length, log);
            LOG(LOGTYPE_ERROR, "Shader compilation error in ",(type==GL_VERTEX_SHADER?"Vertex":"Fragment"),"shader: \n"+std::string(log)+"\n");
            glDeleteShader(shader);
            shader = -1;
        }
        return shader;
    }
}