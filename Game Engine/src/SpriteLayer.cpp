#include "SpriteLayer.hpp"

namespace PrEngine
{

    SpriteLayer::SpriteLayer(long camera_handle)
    {
        this->camera_handle = camera_handle;
        this->name = "Sprite";
    }

    void SpriteLayer::start()
    {
    }

    void SpriteLayer::update()
    {
        insertion_sort(sprite_list, sprite_list.size());
        Entity* camera = entity_management_system->get_entity(camera_handle);

        DirectionalLight* light = (DirectionalLight*)entity_management_system->get_entity_with_component(COMP_LIGHT);
        /*
        for(std::vector<Sprite*>::iterator it = sprite_list.begin(); it != sprite_list.end(); it++ )
        {
            Graphics& grp = (*it)->graphics;
            //Matrix4x4<float> mvp = (projection) * (*(grp.model)) ;
            
            for(int i=0; i < grp.elements.size(); i++)
            {
                grp.elements[i].material->diffuse_texture->Bind(0);
                grp.elements[i].vao.Bind();
                grp.elements[i].ibo.Bind();
                

                //std::cout<<"before: "<<grp.elements[i].material.uniform_locations["u_MVP"]  <<std::endl;
                //if(grp.elements[i].material.uniform_locations["u_sampler2d"] != -1)
                //GL_CALL(
                //    glUniform1i(grp.elements[i].material.uniform_locations["u_sampler2d"], 0))
                if(grp.elements[i].material->uniform_locations["u_Dir_Light"] != -1)
                GL_CALL(
                    glUniform3f(grp.elements[i].material->uniform_locations["u_Dir_Light"], light->direction.x, light->direction.y, light->direction.z))
                
                // models and normals should be same size
                for(int j=0; j<grp.models.size() ; j++) 
                {

                    Camera* cam_component = (Camera*)(camera->components[COMP_CAMERA]);
                    Matrix4x4<float> mvp = (cam_component->projection_matrix) * cam_component->view_matrix * (*(grp.models[j])) ;

                    if(grp.elements[i].material->uniform_locations["u_MVP"] != -1)
                    GL_CALL(
                        glUniformMatrix4fv(grp.elements[i].material->uniform_locations["u_MVP"],1, GL_TRUE, mvp.data))

                    if(grp.elements[i].material->uniform_locations["u_Normal_M"] != -1)
                    GL_CALL(
                        glUniformMatrix4fv(grp.elements[i].material->uniform_locations["u_Normal_M"],1, GL_TRUE, grp.normals[j]->data ))
                    GL_CALL(
                        glUniform2f(grp.elements[i].material->uniform_locations["u_Panning"],grp.elements[i].material->panning.x, grp.elements[i].material->panning.y);
                    )
                    GL_CALL(
                        glUniform2f(grp.elements[i].material->uniform_locations["u_Tiling"],grp.elements[i].material->tiling.x, grp.elements[i].material->tiling.y);
                    )
                
                    GL_CALL(
                    //glDrawArrays(GL_TRIANGLES,0, grp.elements[i].num_of_triangles*3))
                    glDrawElements(GL_TRIANGLES, grp.elements[i].ibo.count, GL_UNSIGNED_INT, nullptr));
                
                }

                //grp.ibo[i].Bind();

                //GL_CALL(
                //    glUniform1f((*it)->material.uniform_locations["u_red"], 1.f))
               
                
                grp.elements[i].ibo.Unbind();
                grp.elements[i].vao.Unbind();
                //grp.ibo[i].Unbind();
            }
        }
        */
    }

    void SpriteLayer::end()
    {

    }
    
    SpriteLayer::~SpriteLayer()
    {
        LOG(LOGTYPE_GENERAL, "Deleting sprite layer");
    }


    void insertion_sort(std::vector<Sprite*>& arr, int n)  
    {  
        int i, j;  
        //LOG(LOGTYPE_GENERAL, std::to_string(n));
        Sprite* key;
        for (i = 1; i < n; i++) 
        {  
            key = arr[i];  
            j = i - 1;  
    
            /* Move elements of arr[0..i-1], that are  
            greater than key, to one position ahead  
            of their current position */
            while (j >= 0 && arr[j]->order > key->order) 
            {  
                arr[j + 1] = arr[j];
                j = j - 1;  
            }  
            arr[j + 1] = key;  
        }  
    } 

}