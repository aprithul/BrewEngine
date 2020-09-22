#include "Texture.hpp"

namespace PrEngine
{
    int Texture::texture_create_status;

    std::unordered_map<std::string, TextureData> Texture::texture_data_library;
    std::unordered_map<std::string, Texture*> Texture::texture_library;

    Texture::Texture(const char* path)
    {
        texture_create_status = 0;
        stbi_set_flip_vertically_on_load(true);
		this->path = path;
        if(texture_data_library.count(path) > 0)
        {
            TextureData td = texture_data_library[path];
            data = td.data;
            width = td.width;
            height = td.height;
            no_of_channels = td.no_of_channels;
        }
        else
        {
            data = stbi_load(path,&width, &height, &no_of_channels, 0);
            if(data!=nullptr)
            {
                TextureData td;
                td.data = data;
                td.width = width;
                td.height = height;
                td.no_of_channels = no_of_channels;
                texture_data_library[path] = td;
            }

        }


        if(data == NULL){
            LOG(LOGTYPE_ERROR, "Couldn't create texture ", path);
        }
        else
        {
            LOG(LOGTYPE_GENERAL, "Image ",std::string(path)," loaded");

            GL_CALL(glGenTextures(1, &id))
            GL_CALL(glActiveTexture(GL_TEXTURE0))
            GL_CALL(glBindTexture(GL_TEXTURE_2D, id))
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT))
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT))
            
            GLenum type = GL_RGBA;
            switch(no_of_channels)
            {
                case 1:type = GL_R;break;
                case 2:type = GL_RG;break;
                case 3:type = GL_RGB;break;
                case 4:type = GL_RGBA;break;
            }
            LOG(LOGTYPE_GENERAL, "Number of channels ", std::to_string(type));
            GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, type, GL_UNSIGNED_BYTE, data))
            GL_CALL(glBindTexture(GL_TEXTURE_2D, 0))
            texture_create_status = 1;
            Texture::texture_library[std::string(path)] = this;
        }
    }

    Texture::Texture()
    {

    }    

    void Texture::load_default_texture()
    {
    	Texture* _tex = new Texture(get_resource_path("default.jpg").c_str());
    }

    Texture* Texture::load_texture(const std::string& path)
    {
    	Texture* _tex = new Texture(path.c_str());
    	if(Texture::texture_create_status == 0)
    		return nullptr;
    	else
    		return _tex;
    }

    void Texture::delete_all_texture_data()
    {
    	LOG(LOGTYPE_GENERAL, "Deleteing all texture data");
    	for(std::unordered_map<std::string, TextureData>::iterator it = Texture::texture_data_library.begin(); it != Texture::texture_data_library.end(); it++)
		{
    		if(it->second.data!=NULL)
    		{
    			it->second.Delete();
    		}
		}
    	Texture::texture_data_library.clear();
    }

    void Texture::delete_all_textures()
    {
    	LOG(LOGTYPE_GENERAL, "Deleteing all textures");
    	for(std::unordered_map<std::string, Texture*>::iterator it = Texture::texture_library.begin(); it != Texture::texture_library.end(); it++)
    	{
    		LOG(LOGTYPE_GENERAL, "Deleting : ", std::to_string(it->second->id));
    		delete it->second;
    	}
    	Texture::texture_library.clear();
    }

    Texture::~Texture()
    {
        Unbind();
        GL_CALL(
            glDeleteTextures(1, &id))

    }

    void Texture::Bind(int slot)
    {
        GL_CALL(
            glActiveTexture(GL_TEXTURE0 + slot))
        GL_CALL(
            glBindTexture(GL_TEXTURE_2D, id))
    }

    void Texture::Unbind()
    {
        GL_CALL(
            glBindTexture(GL_TEXTURE_2D, 0))
    }

    void TextureData::Delete()
    {
        if(data != NULL)
        {
			LOG(LOGTYPE_GENERAL,"Deleting texture data");
        	stbi_image_free(data);
        }
    }

}
