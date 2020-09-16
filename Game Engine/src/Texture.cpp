#include "Texture.hpp"

namespace PrEngine
{
    int texture_create_status;
    std::unordered_map<std::string, Texture*> texture_library;
    std::unordered_map<std::string, TextureData> texture_data_library;

    Texture::Texture(const char* path)
    {
        texture_create_status = 0;
        stbi_set_flip_vertically_on_load(true);
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
            LOG(LOGTYPE_ERROR, "Couldn't create texture");
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
            texture_library[std::string(path)] = this;
        }
    }

    Texture::Texture()
    {

    }    

    Texture::~Texture()
    {
        Unbind();
        GL_CALL(
            glDeleteTextures(1, &id))
        if(data != NULL)
            stbi_image_free(data);
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


    TextureCubeMap::TextureCubeMap( const std::vector<std::string>& paths)
    {
        GL_CALL(glGenTextures(1, &id))
        GL_CALL(glActiveTexture(GL_TEXTURE0+1))
        GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, id))
        stbi_set_flip_vertically_on_load(false);

        for(int i=0; i<paths.size(); i++)
        {
            std::string path = paths[i];
            texture_create_status = 0;

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
                data = stbi_load(path.c_str(),&width, &height, &no_of_channels, 0);
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
                texture_create_status = 0;
                LOG(LOGTYPE_ERROR, "Couldn't create texture : ", path);
                return;
            }
            else
            {
                LOG(LOGTYPE_GENERAL, "Image ",std::string(path)," loaded");
                GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data))
                texture_create_status = 1;
            }
        }
        GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
        GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
        GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE))
        GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
        GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))

        GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0))

        LOG(LOGTYPE_GENERAL, "Cubemap texture created successfully");
        texture_create_status = 1;
        
    }

    void TextureCubeMap::Bind(int slot)
    {
        GL_CALL(glActiveTexture(GL_TEXTURE0+slot))
        GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, id))
    }

    void TextureCubeMap::Unbind()
    {
        GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0))
    }
    TextureCubeMap::~TextureCubeMap()
    {

    }

    void delete_texture_from_library()
    {
        
    }
}
