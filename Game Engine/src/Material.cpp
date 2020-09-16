#include "Material.hpp"

namespace PrEngine
{

    std::unordered_map<std::string, Material*> material_library;
    std::unordered_map<std::string, Shader> shader_library;

    /*Material::()
    {

    }*/

    Material::Material(const std::string& shader_path, const std::string& diffuse_tex_path, TextureCubeMap& env_map, const std::string& name)
    {
        // only create new texture on gpu if texture doesn't exist already
        tiling = Vector2<float>(1,1);
        panning = Vector2<float>(0,0);
        
        environment_map_texture = &env_map;

        std::unordered_map<std::string, Texture*>::iterator _tex_it = texture_library.find(diffuse_tex_path);
        if(_tex_it == texture_library.end()) // texture not in library, so create
        {
            Texture* _tex =  new Texture(diffuse_tex_path.c_str());
            if(texture_create_status == 0){    // creating texture failed, so assign default
                delete _tex;
                diffuse_texture = texture_library[get_resource_path("default.jpg")];
            }
            else // successfully created texture, store in library and assign that
            {
                diffuse_texture = _tex;

            }
        }
        else    // texture found in library, so assign that
            diffuse_texture = texture_library[diffuse_tex_path];

        std::unordered_map<std::string, Shader>::iterator _shader_it = shader_library.find(shader_path);
        if(_shader_it == shader_library.end())
        {
            // create shader, probably can be shared, will check later
            this->source_file_path = std::string(shader_path);
            make_shader_program(this->source_file_path);
            
            shader_library[shader_path] = shader_program;
        }
        else
            shader_program = _shader_it->second;
        
        /*load_uniform_location("u_sampler2d");
        load_uniform_location("u_env_map");
        load_uniform_location("u_Model");
        load_uniform_location("u_View");
        load_uniform_location("u_Projection");
        load_uniform_location("u_Camera_Position");
        load_uniform_location("u_Normal_M");
        load_uniform_location("u_Dir_Light");
        load_uniform_location("u_Tiling");
        load_uniform_location("u_Panning");*/

    }


    Material::Material(const std::string& shader_path, const std::string& diffuse_tex_path,  const std::string& name)
    {
        // only create new texture on gpu if texture doesn't exist already
        tiling = Vector2<float>(1,1);
        panning = Vector2<float>(0,0);
        environment_map_texture = nullptr;

        std::unordered_map<std::string, Texture*>::iterator _tex_it = texture_library.find(diffuse_tex_path);
        if(_tex_it == texture_library.end()) // texture not in library, so create
        {
            Texture* _tex =  new Texture(diffuse_tex_path.c_str());
            if(texture_create_status == 0){    // creating texture failed, so assign default
                delete _tex;
                diffuse_texture = texture_library[get_resource_path("default.jpg")];
            }
            else // successfully created texture, store in library and assign that
            {
                diffuse_texture = _tex;

            }
        }
        else    // texture found in library, so assign that
            diffuse_texture = texture_library[diffuse_tex_path];

        std::unordered_map<std::string, Shader>::iterator _shader_it = shader_library.find(shader_path);
        if(_shader_it == shader_library.end())
        {
            // create shader, probably can be shared, will check later
            this->source_file_path = std::string(shader_path);
            make_shader_program(this->source_file_path);
            
            shader_library[shader_path] = shader_program;
        }
        else{
            shader_program = _shader_it->second;
        }

    }

    // will create a cubemap texture from the 6 cubemap texture paths
    Material::Material(const std::string& shader_path, const std::vector<std::string>& cubemap_tex_path,const std::string& name)
    {
        // only create new texture on gpu if texture doesn't exist already
        tiling = Vector2<float>(1,1);
        panning = Vector2<float>(0,0);
        environment_map_texture = nullptr;

        std::string cubemap_file_name = "";
        for(int i=0; i<cubemap_tex_path.size(); i++)
        {
            cubemap_file_name += cubemap_tex_path[i];
        }
        std::cout<<"concatanated file name: "<<cubemap_file_name<<std::endl;

        std::unordered_map<std::string, Texture*>::iterator _tex_it = texture_library.find(cubemap_file_name);
        if(_tex_it == texture_library.end()) // texture not in library, so create
        {
            Texture* _tex =  new TextureCubeMap(cubemap_tex_path);
            if(texture_create_status == 0){    // creating texture failed, so assign default
                delete _tex;
                //diffuse_texture = texture_library[get_resource_path("default.jpg")];
            }
            else // successfully created texture, store in library and assign that
            {
                diffuse_texture = _tex;

            }
        }
        else    // texture found in library, so assign that
            diffuse_texture = texture_library[cubemap_file_name];

        std::unordered_map<std::string, Shader>::iterator _shader_it = shader_library.find(shader_path);
        if(_shader_it == shader_library.end())
        {
            // create shader, probably can be shared, will check later
            this->source_file_path = std::string(shader_path);
            make_shader_program(this->source_file_path);

            
            shader_library[shader_path] = shader_program;
        }
        else
            shader_program = _shader_it->second;

        //load_uniform_location("u_sampler_cube");
        //load_uniform_location("u_MVP");
        

    }

    void Material::Bind()
    {
        GL_CALL(
            glUseProgram(shader_program.id))
        diffuse_texture->Bind(0);
        if(environment_map_texture != nullptr)
            environment_map_texture->Bind(1);
    }

    void Material::Unbind()
    {
        GL_CALL(
            glUseProgram(0))
        diffuse_texture->Unbind();
        if(environment_map_texture != nullptr)
            environment_map_texture->Unbind();
    }

    void Material::Delete()
    {
        GL_CALL(
            glDeleteProgram(shader_program.id))

        for(std::unordered_map<std::string, Texture*>::iterator it=texture_library.begin(); it!=texture_library.end(); it++)
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
        }
        
        //delete diffuse_texture;
    }

    Material::~Material()
    {
        
    }

    void Material::load_uniform_location(const std::string& uniform, const std::string& type)
    {
        GLint loc = -1;
        GL_CALL(
            loc = glGetUniformLocation(shader_program.id, uniform.c_str()))
        
        std::cout<<"Location: "<<uniform<<" , "<<loc<<std::endl;
        //if(loc != -1)
        shader_program.uniform_locations[uniform] = {type, loc};
    }

    void Material::parse_shader(const std::string& source)
    {
        int pos = 0;
        while((pos = source.find("uniform",pos)) != std::string::npos)
        {
            int prev = 1;
            while(pos-prev>=0 && source[pos-prev]==' ')
                prev++;
            if(pos-prev<0 || source[pos-prev] !='/') // ignore comments, ignores spaces
            {
                std::string u_type = ""; 
                std::string u_name = "";
                
                int start = pos+8; // "uniform" length is 7
                int i = 0;
                for(; source[start+i]!=' '; i++);
                
                u_type = source.substr(start,i);

                //skip through spaces
                start = start+i;
                while(source[start]==' ')
                    start++;
                
                i = 0;
                for(; !std::isspace(source[start+i]) && source[start+i]!=';'; i++);
                u_name = source.substr(start, i);
                LOG(LOGTYPE_GENERAL, u_type, " ",u_name);
                load_uniform_location(u_name, u_type);
            }

            pos++;
        }
    }

    bool Material::make_shader_program(const std::string& path)
    {
        std::string _source = read_file(get_resource_path(path).c_str());
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
        if(shader_program.id == 0)
        {
            std::cout<<"Couldn't create shader program"<<std::endl;
            return false;
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
                int length;
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

                return true;
            }
        }
        return false;
    }

    GLuint Material::make_shader( GLenum type,  const std::string& source)
    {
        GLuint shader =  glCreateShader(type);
        if(shader == 0)
            std::cout<<"Couldn't create shader"<<std::endl;
            
        const char* _source = source.c_str();
        glShaderSource(shader,1, &(_source) , NULL);
        glCompileShader(shader);
        
        // check
        GLint compilation_status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_status);
        if(compilation_status != GL_TRUE)
        {
            int length;
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
