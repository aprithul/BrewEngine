#include "GeometryLayer.hpp"

namespace PrEngine
{

    GeometryLayer::GeometryLayer(long camera_handle)
    {
        this->camera_handle = camera_handle;
        this->name = "Geometry";
    }
    
    void GeometryLayer::start()
    {
        
    }

    void GeometryLayer::update()
    {

        //Entity* camera = cameras[0].get_entity(camera_handle);

        //DirectionalLight* light = (DirectionalLight*)(((Entity*)EntityManagementSystem::entity_management_system->get_entity_with_component(COMP_LIGHT))->components[COMP_LIGHT]);

		DirectionalLight _light;
		Camera _camera;
		Vector3<Float_32> _dir = transforms[_light.id].get_forward();

        for(Uint_32 _i = 0; _i<MAX_ENTITY_COUNT; _i++)
        {
			if (is_entity_valid(_i))
			{            //Matrix4x4<Float_32> mvp = (projection) * (*(grp->model)) ;
				graphics[_i].element.material->Bind();
				//std::cout<<"before: "<<grp->elements[i].material.uniform_locations["u_MVP"]  <<std::endl;

				std::unordered_map<std::string, std::pair<std::string, GLuint>>& m = graphics[_i].element.material->shader->uniform_locations;


				for (std::unordered_map<std::string, std::pair<std::string, GLuint>>::iterator it = m.begin(); it != m.end(); it++)
				{

					if (it->first == "u_sampler2d")
					{
						GL_CALL(
							glUniform1i(it->second.second, 0))
					}

					if (it->first == "u_env_map")
					{
						GL_CALL(
							glUniform1i(it->second.second, 1))
					}

					if (it->first == "u_sampler_cube")
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
							glUniform1f(it->second.second, light.ambient)
						)
					}

					if (it->first == "u_Specular_Strength")
					{
						GL_CALL(
							glUniform1f(it->second.second, light.specular)
						)
					}
					// models and normals should be same size
					//for(Int_32 j=0; j<grp->models.size() ; j++) 
					//{

					Camera* cam_component = (Camera*)(camera->components[COMP_CAMERA]);
					//Matrix4x4<Float_32> mvp = (cam_component->projection_matrix) * cam_component->view_matrix * (*(grp->models[j])) ;


					if (it->first == "u_skybox")
					{
						GL_CALL(glUniform1i(it->second.second, 0))
							//mvp = (cam_component->projection_matrix) * _view * (*(grp->models[j])) ;

					}

					if (it->first == "u_Model")
					{
						GL_CALL(
							glUniformMatrix4fv(it->second.second, 1, GL_TRUE, grp->transform->transformation.data))
					}

					if (it->first == "u_View")
					{
						GL_CALL(
							glUniformMatrix4fv(it->second.second, 1, GL_TRUE, cam_component->view_matrix.data))
					}

					if (it->first == "u_View_t")
					{
						Matrix4x4<Float_32> _view = Matrix4x4<Float_32>(cam_component->view_matrix);
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
							glUniformMatrix4fv(it->second.second, 1, GL_TRUE, (cam_component->projection_matrix).data))
					}

					if (it->first == "u_Camera_Position")
					{
						Vector3<Float_32> cam_pos = cam_component->transform->get_position();
						GL_CALL(
							glUniform3f(it->second.second, cam_pos.x, cam_pos.y, cam_pos.z))
					}

					if (it->first == "u_Normal_M")
					{
						GL_CALL(
							glUniformMatrix4fv(it->second.second, 1, GL_TRUE, grp->transform->rotation_transformation.data))
					}

					if (it->first == "u_Panning")
					{
						GL_CALL(
							glUniform2f(it->second.second, grp->element.material->panning.x, grp->element.material->panning.y);
						)
					}

					if (it->first == "u_Tiling")
					{
						GL_CALL(
							glUniform2f(it->second.second, grp->element.material->tiling.x, grp->element.material->tiling.y);
						)
					}


					//}

					//grp->ibo[i].Bind();

					//GL_CALL(
					//    glUniform1f((*it)->material.uniform_locations["u_red"], 1.f))
				}

				grp->element.vao.Bind();
				grp->element.ibo.Bind();
				GL_CALL(
					//glDrawArrays(GL_TRIANGLES,0, grp->elements[i].num_of_triangles*3))
					glDrawElements(GL_TRIANGLES, grp->element.ibo.count, GL_UNSIGNED_INT, nullptr));
				grp->element.vao.Unbind();
				grp->element.ibo.Unbind();

				grp->element.material->Unbind();
			}
            //grp->ibo[i].Unbind();
        }
    }

    void GeometryLayer::end()
    {

    }

    GeometryLayer::~GeometryLayer()
    {
        LOG(LOGTYPE_GENERAL, "Deleting geometry layer");

    }

}
