#include "Texture.hpp"

namespace PrEngine
{
    Int_32 Texture::texture_create_status;

    std::unordered_map<std::string, TextureData> Texture::texture_data_library;
    std::vector<Texture> Texture::texture_library;
	std::vector<std::string> Texture::texture_names;
	Uint_32 Texture::next_bind_unit = 0;
    Texture::Texture(const std::string& path)
    {
        texture_create_status = 0;
        stbi_set_flip_vertically_on_load(true);
		this->path = 0;
		//this->path = std::string(path);
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
			std::string resource_path = get_resource_path(path);
            data = stbi_load(resource_path.c_str(),&width, &height, &no_of_channels, 0);
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

			
			bind_unit = next_bind_unit;
			next_bind_unit++;
            GL_CALL(glGenTextures(1, &id))
            GL_CALL(glActiveTexture(GL_TEXTURE0+bind_unit))
				LOG(LOGTYPE_GENERAL, "Image ", std::string(path), " loaded : "+ std::to_string(bind_unit));

            GL_CALL(glBindTexture(GL_TEXTURE_2D, id))
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR))
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
			LOG(LOGTYPE_GENERAL, "Number of channels ", std::to_string(no_of_channels));
			LOG(LOGTYPE_GENERAL, "Number of channels ", std::to_string(type));
			GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, type, GL_UNSIGNED_BYTE, data))
			GL_CALL(glGenerateMipmap(GL_TEXTURE_2D))

            //GL_CALL(glBindTexture(GL_TEXTURE_2D, 0))
            texture_create_status = 1;
        }
    }


    Texture* Texture::load_default_texture()
    {
    	Texture* _tex = new Texture("default.jpg");
		if (texture_create_status)
			return _tex;
		else
		{
			delete _tex;
			return nullptr;
		}
    }

	Uint_32 Texture::load_texture(const std::string& path)
    {
		//Texture* _tex;
		//std::unordered_map<std::string, Texture*>::iterator _tex_it = Texture::texture_library.find(path);
	
		Int_32 present_at = -1;
		for (int _i = 0; _i < texture_names.size(); _i++)
		{
			if (texture_names[_i] == path)
			{
				present_at = _i;
				break;
			}
		}
		
		Uint_32 texture_id = 0;
		//if (_tex_it == Texture::texture_library.end()) // texture not in library, so create
		if (present_at == -1)
		{
			//_tex = new Texture(path.c_str());
			texture_library.emplace_back(path.c_str());
			if (Texture::texture_create_status == 0)     // creating texture failed, so assign default
			{
				Texture* _tex = &texture_library.back();
				delete _tex;
				texture_library.pop_back();

				//_tex = load_default_texture();	//couldn't load, so try loading default
				//if (texture_create_status == 0)	//if still fails, will return nullptr
				//{
				//	delete _tex;
				//	_tex = nullptr;
				//}
			}
			else
			{
				texture_id = texture_library.size() - 1;

				texture_names.push_back(path);
				texture_library.back().path = texture_names.size() - 1;
			}
		}
		else    // texture found in library, so assign that
			texture_id = present_at;

		return texture_id;
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
    	for(auto& tex:texture_library)
    	{
    		LOG(LOGTYPE_GENERAL, "Deleting : ", std::to_string(tex.id));
			tex.Delete();
    		//delete &tex;
    	}
    	Texture::texture_library.clear();
		Texture::texture_names.clear();
    }

	void Texture::Delete()
	{
		Unbind();
		GL_CALL(
			glDeleteTextures(1, &id))
	}

    Texture::~Texture()
    {


		//Texture::texture_library.erase(this->path);

    }

    void Texture::Bind(Int_32 unit)
    {
        GL_CALL(
            glActiveTexture(GL_TEXTURE0 + unit))
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
