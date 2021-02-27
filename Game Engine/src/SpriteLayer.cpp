#include "SpriteLayer.hpp"
#include "RendererOpenGL2D.hpp"
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
		Uint_32 camera_id = entity_management_system->get_active_camera();
		if (!camera_id)
			return;

		Camera _camera = cameras[camera_id];
		DirectionalLight _light = directional_lights[1];
		Vector3<Float_32> _cam_pos = get_transform(_camera.id_transform).position;
		Vector3<Float_32> _dir = get_transform(_light.id_transform).get_forward();


		if (renderer->lines_buffer.size() > 0)
		{
			GraphicsElement& element = renderer->line_graphic.element;
			Material* mat = Material::get_material(element.material);
			if (mat == nullptr)
			{
				LOG(LOGTYPE_ERROR, "Couldn't find material");
				return;
			}

			mat->Bind();
			element.vao.Bind();
			element.ibo.Bind();

			auto& u_locs = Shader::shader_library[mat->shader].uniform_locations;
			for (auto& u : u_locs)
			{
				switch (u.first)
				{
				case ShaderUniformName::u_View:
					GL_CALL(
						glUniformMatrix4fv(u.second.second, 1, GL_TRUE, _camera.view_matrix.data);
					)
					break;
				case ShaderUniformName::u_Projection:
					GL_CALL(
						glUniformMatrix4fv(u.second.second, 1, GL_TRUE, _camera.projection_matrix.data);
					)
					break;
				default:
					break;
				}
			}

			GL_CALL(
				glDrawElements(GL_LINES, renderer->line_graphic.element.ibo.count, GL_UNSIGNED_INT, nullptr));

			element.vao.Unbind();
			element.ibo.Unbind();
			mat->Unbind();

			renderer->lines_buffer.clear();
			renderer->lines_indices.clear();
			element.vbo.Delete();
			element.ibo.Delete();
			element.vao.Delete();
		}


		for (Uint_32 _i = 0; _i < MAX_GRAPHIC_COUNT; _i++)
		{
			if (graphics_entity_id[_i])//  is_valid(graphic_active_status, graphics[_i].entity))
			{
				//UpdateTransforms(transform);
				//Matrix4x4<Float_32> mvp = (projection) * (*(grp->model)) ;
				auto& graphic = graphics[_i];
				auto& transform = transforms[graphic.id_transform];

				GraphicsElement& element = graphic.element;
				Material* mat = Material::get_material(element.material);
				if (mat == nullptr)
				{
					LOG(LOGTYPE_ERROR, "Couldn't find material");
					continue;
				}

				//graphic.bounding_rect.x = transform.position.x;
				//graphic.bounding_rect.y = transform.position.y;
				//std::cout<<"before: "<<grp->element.material.uniform_locations["u_MVP"]  <<std::endl;
				
				mat->Bind();
				if (mat == nullptr)
				{
					LOG(LOGTYPE_ERROR, "Material is null");
					continue;

				}
				Shader* shader = Shader::get_shader(mat->shader);
				if (shader == nullptr)
				{
					LOG(LOGTYPE_ERROR, "Shader couldn't be found");
					continue;
				}
				auto& uniform_loc = shader->uniform_locations;


				for (auto& it : uniform_loc)
				{

					switch (it.first)
					{
					case ShaderUniformName::u_sampler2d:
					{
						Texture* tex = Texture::get_texture(mat->diffuse_texture);
						GL_CALL(
							glUniform1i(it.second.second, tex->bind_unit))
					}
						break;
					case ShaderUniformName::u_Dir_Light:
						GL_CALL(
							glUniform3f(it.second.second, _dir.x, _dir.y, _dir.z))
						break;
					case ShaderUniformName::u_Model:
						GL_CALL(
							glUniformMatrix4fv(it.second.second, 1, GL_TRUE, transform.transformation.data))
						break;
					case ShaderUniformName::u_View:
						GL_CALL(
							glUniformMatrix4fv(it.second.second, 1, GL_TRUE, _camera.view_matrix.data))
						break;
					case ShaderUniformName::u_View_t:
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
							glUniformMatrix4fv(it.second.second, 1, GL_TRUE, _view.data))
					}
						break;
					case ShaderUniformName::u_Projection:
						GL_CALL(
							glUniformMatrix4fv(it.second.second, 1, GL_TRUE, _camera.projection_matrix.data))
						break;
					case ShaderUniformName::u_Camera_Position:
						GL_CALL(
							glUniform3f(it.second.second, _cam_pos.x, _cam_pos.y, _cam_pos.z))
						break;
					case ShaderUniformName::u_Normal_M:
						GL_CALL(
							glUniformMatrix4fv(it.second.second, 1, GL_TRUE, transform.rotation_transformation.data))
						break;
					case ShaderUniformName::u_Panning:
						GL_CALL(
							glUniform2f(it.second.second, mat->panning.x, mat->panning.y))
						break;
					case ShaderUniformName::u_Tiling:
						GL_CALL(
							glUniform2f(it.second.second, mat->tiling.x, mat->tiling.y))
						break;
					case ShaderUniformName::u_Diffuse_Color:
						GL_CALL(
							glUniform3f(it.second.second, mat->diffuse_color.x, mat->diffuse_color.y, mat->diffuse_color.z))
						break;
					case ShaderUniformName::u_Outline_Color:
						GL_CALL(
							glUniform4f(it.second.second, graphic.outline_color.x, graphic.outline_color.y, graphic.outline_color.z, graphic.outline_alpha))
						break;
					case ShaderUniformName::u_Ambient_Strength:
						GL_CALL(
							glUniform1f(it.second.second, 1.f))
							break;
					default:
						break;
					}



					//}

					//grp->ibo[i].Bind();

					//GL_CALL(
					//    glUniform1f((*it)->material.uniform_locations["u_red"], 1.f))
				}
				element.vao.Bind();
				element.ibo.Bind();
				GL_CALL(
					//glDrawArrays(GL_TRIANGLES,0, grp->element.num_of_triangles*3))
					glDrawElements(GL_TRIANGLES, element.ibo.count, GL_UNSIGNED_INT, nullptr));
				element.vao.Unbind();
				element.ibo.Unbind();
				mat->Unbind();

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
