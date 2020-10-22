#include "SpriteLayer.hpp"

namespace PrEngine
{

    SpriteLayer::SpriteLayer()
    {
        this->name = "Sprite";

    }

    void SpriteLayer::start()
    {
    }

	void SpriteLayer::UpdateTransforms(Transform3D* transform)
	{
		/*if (transform->parent != nullptr)
		{
			UpdateTransforms(transform->parent);
		}

		if (transform->dirty)
		{
			transform->update_transformation();
		}*/
	}

    void SpriteLayer::update()
    {
        //insertion_sort(sprite_list, sprite_list.size());
		DirectionalLight _light = directional_lights[1];
		Camera _camera = cameras[1];
		Vector3<Float_32> _cam_pos = get_transform(_camera.id_transform).position;
		Vector3<Float_32> _dir = get_transform(_light.id_transform).get_forward();

		for (Uint_32 _i = 0; _i < MAX_GRAPHIC_COUNT; _i++)
		{
			if (graphic_active_status[_i])//  is_valid(graphic_active_status, graphics[_i].entity))
			{
				//UpdateTransforms(transform);
				//Matrix4x4<Float_32> mvp = (projection) * (*(grp->model)) ;

				graphics[_i].element.material->Bind();
				//std::cout<<"before: "<<grp->element.material.uniform_locations["u_MVP"]  <<std::endl;

				std::unordered_map<std::string, std::pair<std::string, GLuint>>& m = graphics[_i].element.material->shader->uniform_locations;


				for (std::unordered_map<std::string, std::pair<std::string, GLuint>>::iterator it = m.begin(); it != m.end(); it++)
				{

					if (it->first == "u_sampler2d")
					{
						GL_CALL(
							glUniform1i(it->second.second, 0))
					}

					if (it->first == "u_Dir_Light")
					{

						GL_CALL(
							glUniform3f(it->second.second, _dir.x, _dir.y, _dir.z))
					}

					if (it->first == "u_Ambient_Strength")
					{
						//LOG(LOGTYPE_ERROR, "Dir: ", std::to_string(light->direction.x));
						GL_CALL(
							glUniform1f(it->second.second, _light.ambient)
						)
					}

					if (it->first == "u_Specular_Strength")
					{
						GL_CALL(
							glUniform1f(it->second.second, _light.specular)
						)
					}
					// models and normals should be same size
					//for(Int_32 j=0; j<grp->models.size() ; j++)
					//{

					//Camera* cam_component = (Camera*)(camera->components[COMP_CAMERA]);
					//Matrix4x4<Float_32> mvp = (cam_component->projection_matrix) * cam_component->view_matrix * (*(grp->models[j])) ;


					if (it->first == "u_Model")
					{

						GL_CALL(
							glUniformMatrix4fv(it->second.second, 1, GL_TRUE, transforms[graphics[_i].id_transform].transformation.data))
					}

					if (it->first == "u_View")
					{
						GL_CALL(
							glUniformMatrix4fv(it->second.second, 1, GL_TRUE, _camera.view_matrix.data))
					}

					if (it->first == "u_View_t")
					{
						Matrix4x4<Float_32> _view = Matrix4x4<Float_32>(_camera.view_matrix);
						_view.data[3] = 0;
						_view.data[7] = 0;
						_view.data[11] = 0;
						_view.data[12] = 0;
						_view.data[13] = 0;
						_view.data[14] = 0;
						_view.data[15] = 1;

						GL_CALL(
							glUniformMatrix4fv(it->second.second, 1, GL_TRUE, _view.data))
					}

					if (it->first == "u_Projection")
					{
						GL_CALL(
							glUniformMatrix4fv(it->second.second, 1, GL_TRUE, _camera.projection_matrix.data))
					}

					if (it->first == "u_Camera_Position")
					{
						
						GL_CALL(
							glUniform3f(it->second.second, _cam_pos.x, _cam_pos.y, _cam_pos.z))
					}

					if (it->first == "u_Normal_M")
					{
						GL_CALL(
							glUniformMatrix4fv(it->second.second, 1, GL_TRUE, transforms[graphics[_i].id_transform].rotation_transformation.data))
					}

					if (it->first == "u_Panning")
					{
						GL_CALL(
							glUniform2f(it->second.second, graphics[_i].element.material->panning.x, graphics[_i].element.material->panning.y);
						)
					}

					if (it->first == "u_Tiling")
					{
						GL_CALL(
							glUniform2f(it->second.second, graphics[_i].element.material->tiling.x, graphics[_i].element.material->tiling.y);
						)
					}


					//}

					//grp->ibo[i].Bind();

					//GL_CALL(
					//    glUniform1f((*it)->material.uniform_locations["u_red"], 1.f))
				}
				graphics[_i].element.vao.Bind();
				graphics[_i].element.ibo.Bind();
				GL_CALL(
					//glDrawArrays(GL_TRIANGLES,0, grp->element.num_of_triangles*3))
					glDrawElements(GL_TRIANGLES, graphics[_i].element.ibo.count, GL_UNSIGNED_INT, nullptr));
				graphics[_i].element.vao.Unbind();
				graphics[_i].element.ibo.Unbind();
				graphics[_i].element.material->Unbind();

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


    void insertion_sort(Sprite** arr, Int_32 n)  
    {  
        Int_32 i, j;  
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
