#include "SpriteLayer.hpp"

namespace PrEngine
{

    SpriteLayer::SpriteLayer()
    {
        this->name = "Sprite";
        this->camera = nullptr;
        this->light = nullptr;
    }

    void SpriteLayer::start()
    {
    }

    void SpriteLayer::update()
    {
        insertion_sort(sprite_list, sprite_list.size());
        if(camera == nullptr)
        {
        	camera = EntityManagementSystem::entity_management_system->get_entity_with_component(COMP_CAMERA);
        	if(camera==nullptr)
        	{
        		LOG(LOGTYPE_ERROR, "Camera not found");
        		return;
        	}
        }
        if(light == nullptr)
        {
        	light = EntityManagementSystem::entity_management_system->get_entity_with_component(COMP_LIGHT);
        	if(light==nullptr)
			{
				LOG(LOGTYPE_ERROR, "light not found");
				return;
			}
        }

        for(std::vector<Sprite*>::iterator it = sprite_list.begin(); it != sprite_list.end(); it++ )
        {
            Graphics* grp = (*it)->graphics;
            //Matrix4x4<float> mvp = (projection) * (*(grp->model)) ;
            
            for(int i=0; i < grp->elements.size(); i++)
            {
            	grp->elements[i].material->Bind();
				//std::cout<<"before: "<<grp->elements[i].material.uniform_locations["u_MVP"]  <<std::endl;

				std::unordered_map<std::string, std::pair<std::string,GLuint>>& m = grp->elements[i].material->shader->uniform_locations;


				for( std::unordered_map<std::string, std::pair<std::string,GLuint>>::iterator it = m.begin(); it!=m.end(); it++)
				{

					if( it->first == "u_sampler2d")
					{
						GL_CALL(
							glUniform1i(it->second.second, 0))
					}

					if(it->first == "u_env_map")
					{
						GL_CALL(
							glUniform1i(it->second.second, 1))
					}

					if(it->first == "u_sampler_cube")
					{
						GL_CALL(
							glUniform1i(it->second.second, 0))
					}

					if(it->first == "u_Dir_Light")
					{

						Vector3<float> dir = ((Transform3D*)(EntityManagementSystem::entity_management_system->get_entity_with_component(COMP_LIGHT)->components[COMP_TRANSFORM_3D]))->get_forward(); // must change
						GL_CALL(
							glUniform3f(it->second.second, dir.x, dir.y, dir.z))
					}

					if(it->first == "u_Ambient_Strength")
					{
						//LOG(LOGTYPE_ERROR, "Dir: ", std::to_string(light->direction.x));
						GL_CALL(
							glUniform1f(it->second.second, ((DirectionalLight*)(light->components[COMP_LIGHT]))->ambient)
						)
					}

					if(it->first == "u_Specular_Strength")
					{
						GL_CALL(
							glUniform1f(it->second.second, ((DirectionalLight*)(light->components[COMP_LIGHT]))->specular)
						)
					}
					// models and normals should be same size
					//for(int j=0; j<grp->models.size() ; j++)
					//{

					Camera* cam_component = (Camera*)(camera->components[COMP_CAMERA]);
					//Matrix4x4<float> mvp = (cam_component->projection_matrix) * cam_component->view_matrix * (*(grp->models[j])) ;


					if(it->first == "u_skybox")
					{
						GL_CALL(glUniform1i(it->second.second, 0))
						//mvp = (cam_component->projection_matrix) * _view * (*(grp->models[j])) ;

					}

					if(it->first == "u_Model")
					{

						GL_CALL(
							glUniformMatrix4fv(it->second.second,1, GL_TRUE, grp->model->data))
					}

					if(it->first == "u_View")
					{
						GL_CALL(
							glUniformMatrix4fv(it->second.second, 1, GL_TRUE, cam_component->view_matrix.data) )
					}

					if(it->first == "u_View_t")
					{
						Matrix4x4<float> _view = Matrix4x4<float>(cam_component->view_matrix);
						_view.data[3] = 0;
						_view.data[7] = 0;
						_view.data[11] = 0;
						_view.data[12] = 0;
						_view.data[13] = 0;
						_view.data[14] = 0;
						_view.data[15] = 1;

						GL_CALL(
							glUniformMatrix4fv(it->second.second, 1, GL_TRUE, _view.data) )
					}

					if(it->first == "u_Projection")
					{
						GL_CALL(
							glUniformMatrix4fv(it->second.second,1, GL_TRUE, (cam_component->projection_matrix).data))
					}

					if(it->first == "u_Camera_Position")
					{
						Vector3<float> cam_pos = cam_component->transform.get_position();
						GL_CALL(
							glUniform3f(it->second.second, cam_pos.x, cam_pos.y, cam_pos.z) )
					}

					if(it->first == "u_Normal_M")
					{
						GL_CALL(
							glUniformMatrix4fv(it->second.second,1, GL_TRUE, grp->normal->data ))
					}

					if(it->first =="u_Panning")
					{
						GL_CALL(
							glUniform2f(it->second.second,grp->elements[0].material->panning.x, grp->elements[0].material->panning.y);
						)
					}

					if(it->first =="u_Tiling")
					{
						GL_CALL(
							glUniform2f(it->second.second,grp->elements[0].material->tiling.x, grp->elements[0].material->tiling.y);
						)
					}


					//}

					//grp->ibo[i].Bind();

					//GL_CALL(
					//    glUniform1f((*it)->material.uniform_locations["u_red"], 1.f))
				}

				grp->elements[i].vao.Bind();
				grp->elements[i].ibo.Bind();
				GL_CALL(
					//glDrawArrays(GL_TRIANGLES,0, grp->elements[i].num_of_triangles*3))
					glDrawElements(GL_TRIANGLES, grp->elements[i].ibo.count, GL_UNSIGNED_INT, nullptr));
				grp->elements[i].vao.Unbind();
				grp->elements[i].ibo.Unbind();

				grp->elements[i].material->Unbind();
            }
        }

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
