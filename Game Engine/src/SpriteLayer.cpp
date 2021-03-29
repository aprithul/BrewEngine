#include "SpriteLayer.hpp"
#include "RendererOpenGL2D.hpp"
#include "Math.hpp"
namespace PrEngine
{
	extern GLint max_texture_units;
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

	int draw_calls = 0;
	inline void render_graphic(Graphic& graphic, Matrix4x4<Float_32>& transformation, Camera& _camera, DirectionalLight& _light, Vector3<Float_32> _cam_pos, Vector3<Float_32> _dir)
	{
		
		draw_calls++;
		GraphicsElement& element = graphic.element;
		Material* mat = Material::get_material(element.material);
		if (mat == nullptr)
		{
			LOG(LOGTYPE_ERROR, "Couldn't find material");
			return;
		}

		//graphic.bounding_rect.x = transform.position.x;
		//graphic.bounding_rect.y = transform.position.y;
		//std::cout<<"before: "<<grp->element.material.uniform_locations["u_MVP"]  <<std::endl;

		// only bind texture for the last texture unit. Ideally we want to have fewer units in use
		// to avoid binding all together. Assumption is that textures are bound at consturction and
		// remain bound
		Texture* tex = Texture::get_texture(mat->diffuse_textures[0]);
		//if (tex->bind_unit == max_texture_units - 1)
		mat->Bind();

		//glActiveTexture(GL_TEXTURE0 + tex->bind_unit);
		Shader* shader = Shader::get_shader(mat->shader);
		if (shader == nullptr)
		{
			LOG(LOGTYPE_ERROR, "Shader couldn't be found");
			return;
		}
		//glUseProgram(shader->id);
		GLint* uniform_loc = shader->uniform_locations;


		for (Uint_32 _i = 0; _i<(Int_32)ShaderUniformName::u_count; _i++)
		{
			ShaderUniformName _name = (ShaderUniformName)_i;
			GLuint _loc = uniform_loc[_i];
			if (_loc >= 0)
			{
				switch (_name)
				{
					case ShaderUniformName::u_sampler2d:
					{
						Texture* tex = Texture::get_texture(mat->diffuse_textures[0]);
						//GL_CALL(
						glUniform1i(_loc, tex->bind_unit);//)
					}
					break;
					case ShaderUniformName::u_textures:	//happens with batche graphic, batche graphi will have array of texture ids
					{
						/*GLint texture_bind_units[MAX_TEXTURES] = {};
						int _count = 0;
						for (int _i = 0; _i < MAX_TEXTURES; _i++)
						{
							//if (mat->diffuse_textures[_i] != 0)
							{
								Texture* tex = Texture::get_texture(mat->diffuse_textures[_i]);
								texture_bind_units[_i] = tex->bind_unit;
								_count++;
							}
							//else
							//	break;
						}

						//GL_CALL(
						glUniform1iv(it.second.second, _count, texture_bind_units);//)*/
					}
					break;
					/*case ShaderUniformName::u_Dir_Light:
						GL_CALL(
							glUniform3f(it.second.second, _dir.x, _dir.y, _dir.z))
							break;*/
					case ShaderUniformName::u_Model:
						GL_CALL(
							glUniformMatrix4fv(_loc, 1, GL_TRUE, transformation.data))
							break;
					case ShaderUniformName::u_View:
						GL_CALL(
							glUniformMatrix4fv(_loc, 1, GL_TRUE, _camera.view_matrix.data))
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
							glUniformMatrix4fv(_loc, 1, GL_TRUE, _view.data))
					}
					break;
					case ShaderUniformName::u_Projection:
						GL_CALL(
							glUniformMatrix4fv(_loc, 1, GL_TRUE, _camera.projection_matrix.data))
							break;
						/*case ShaderUniformName::u_Camera_Position:
							GL_CALL(
								glUniform3f(it.second.second, _cam_pos.x, _cam_pos.y, _cam_pos.z))
								break;
						case ShaderUniformName::u_Normal_M:
							GL_CALL(
								glUniformMatrix4fv(it.second.second, 1, GL_TRUE, transform.rotation_transformation.data))
								break;*/
					case ShaderUniformName::u_Panning:
						GL_CALL(
							glUniform2f(_loc, mat->panning.x, mat->panning.y))
							break;
					case ShaderUniformName::u_Tiling:
						GL_CALL(
							glUniform2f(_loc, mat->tiling.x, mat->tiling.y))
							break;
					case ShaderUniformName::u_Diffuse_Color:
						GL_CALL(
							glUniform3f(_loc, mat->diffuse_color.x, mat->diffuse_color.y, mat->diffuse_color.z))
							break;
					case ShaderUniformName::u_Outline_Color:
						GL_CALL(
							glUniform4f(_loc, graphic.outline_color.x, graphic.outline_color.y, graphic.outline_color.z, graphic.outline_alpha))
							break;
					case ShaderUniformName::u_Ambient_Strength:
						GL_CALL(
							glUniform1f(_loc, 1.f))
							break;
					default:
						break;
				}
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
		//mat->Unbind();

	}

	Uint_32 texture_index = 0;
	void SpriteLayer::update()
	{
		//insertion_sort(sprite_list, sprite_list.size());
		Uint_32 camera_id = entity_management_system->get_active_camera();
		if (!camera_id)
			return;

		Camera _camera = cameras[camera_id];
		DirectionalLight _light = directional_lights[1];
		Vector3<Float_32> _cam_pos = transforms[_camera.id_transform].position;
		Vector3<Float_32> _dir = get_transform(_light.id_transform).get_forward();

		//
		//if (renderer->lines_buffer.size() > 0)
		//{
		//	GraphicsElement& element = renderer->line_graphic.element;
		//	Material* mat = Material::get_material(element.material);
		//	if (mat == nullptr)
		//	{
		//		LOG(LOGTYPE_ERROR, "Couldn't find material");
		//		return;
		//	}

		//	mat->Bind();
		//	element.vao.Bind();
		//	element.ibo.Bind();

		//	GLint* uniform_loc = Shader::shader_library[mat->shader].uniform_locations;


		//	for (Uint_32 _i = 0; _i < (Int_32)ShaderUniformName::u_count; _i++)
		//	{
		//		ShaderUniformName _name = (ShaderUniformName)_i;
		//		GLuint _loc = uniform_loc[_i];
		//		if (_loc >= 0)
		//		{
		//			switch (_name)
		//			{
		//			/*case ShaderUniformName::u_Model:
		//				GL_CALL(
		//					glUniformMatrix4fv(_loc, 1, GL_TRUE, Matrix4x4<Float_32>::identity().data);
		//				)
		//					break;*/
		//			case ShaderUniformName::u_View:
		//				GL_CALL(
		//					glUniformMatrix4fv(_loc, 1, GL_TRUE, _camera.view_matrix.data);
		//				)
		//					break;
		//			case ShaderUniformName::u_Projection:
		//				GL_CALL(
		//					glUniformMatrix4fv(_loc, 1, GL_TRUE, _camera.projection_matrix.data);
		//				)
		//					break;
		//			default:
		//				break;
		//			}
		//		}
		//	}

		//	GL_CALL(
		//		glDrawElements(GL_LINES, renderer->line_graphic.element.ibo.count, GL_UNSIGNED_INT, nullptr));

		//	element.vao.Unbind();
		//	element.ibo.Unbind();
		//	mat->Unbind();

		//	renderer->lines_buffer.clear();
		//	renderer->lines_indices.clear();
		//	element.vbo.Delete();
		//	element.ibo.Delete();
		//	element.vao.Delete();
		//}


		for (Uint_32 _i = 0; _i < MAX_GRAPHIC_COUNT; _i++)
		{
			if (graphics_entity_id[_i])//  is_valid(graphic_active_status, graphics[_i].entity))
			{
				//UpdateTransforms(transform);
				//Matrix4x4<Float_32> mvp = (projection) * (*(grp->model)) ;
				auto& graphic = graphics[_i];
				if (graphic.tag != RENDER_UNTAGGED)
					continue;

				auto& transform = transforms[graphic.id_transform];
				render_graphic(graphic, transform.transformation, _camera, _light, _cam_pos, _dir);
				
			}
		}
		
		for (auto& batch : batched_graphics)
		{
			if (batch.tag == RENDER_DYNAMIC)
			{
				std::vector<Vertex> buffer;
				for (int _i=0; _i<batch.graphic_ids.size(); _i++)
				{
					Uint_32 g_id = batch.graphic_ids[_i];

					Graphic& graphic = graphics[g_id];
					Uint_32 material_id = graphic.element.material;
					assert(material_id);
					Material* mat = Material::get_material(material_id);

					Uint_32 animator_id = graphic.id_animator;
					Uint_32 texture_id = 0;
					Matrix4x4<Float_32> anim_tr = Matrix4x4<Float_32>::identity();
					if (animator_id)
					{
						Animator& anim = animators[animator_id];
						anim_tr = anim.translation*anim.rotation*anim.scale;
						texture_id = anim.get_current_animation().frames[anim.current_frame_index].texture;
					}
					else {
						texture_id = mat->diffuse_textures[0];

					}
					texture_index = batch.texture_to_index[texture_id];
					//LOG(LOGTYPE_WARNING, std::to_string(texture_index));
					Texture* tex = Texture::get_texture(texture_id); //Material::material_library[mat_id].diffuse_texture;
					Float_32 x_scale = tex->width;
					Float_32 y_scale = tex->height;
					Float_32 texco_u = clamp(tex->width / (Float_32)MAX_TEXTURE_SIZE, 0.1f, 1.0f);
					Float_32 texco_v = clamp(tex->height / (Float_32)MAX_TEXTURE_SIZE, 0.1f, 1.0f);

					if (x_scale > y_scale)
					{
						x_scale = (x_scale / y_scale);
						y_scale = 1.f;
					}
					else
					{
						y_scale = y_scale / x_scale;
						x_scale = 1.f;
					}
					GraphicEditorData ged = Graphic::editor_data[g_id];
					x_scale *= ged.scale;
					y_scale *= ged.scale;

					Transform3D& transform = transforms[graphic.id_transform];
					Vector3<Float_32> p1 = transform.transformation * anim_tr * Vector3<Float_32>{ 0.5f*x_scale, 0.5f*y_scale, 0.0f };
					Vector3<Float_32> p2 = transform.transformation * anim_tr * Vector3<Float_32>{ -0.5f*x_scale, 0.5f*y_scale, 0.0f };
					Vector3<Float_32> p3 = transform.transformation * anim_tr * Vector3<Float_32>{ -0.5f*x_scale, -0.5f*y_scale, 0.0f };
					Vector3<Float_32> p4 = transform.transformation * anim_tr * Vector3<Float_32>{ 0.5f*x_scale, -0.5f*y_scale, 0.0f };


					// save vertex data for collider
					// no need to modify every frame, 
					// needs changing
				/*	Graphic::vertex_data[g_id][0] = p1;
					Graphic::vertex_data[g_id][1] = p2;
					Graphic::vertex_data[g_id][2] = p3;
					Graphic::vertex_data[g_id][3] = p4;*/
					//----------------------------------//



					Vertex v1 = {
						p1.x, p1.y, p1.z,

						1.0f,1.0f,1.0f,1.0f,

						0.0f,
						0.0f,
						-1.0f,

						texco_u,
						texco_v,

						texture_index

					};

					Vertex v2 = {
						p2.x, p2.y, p2.z,


						1.0f,
						1.0f,
						1.0f,
						1.0f,

						0.0f,
						0.0f,
						-1.0f,

						0.f,
						texco_v,

						texture_index
					};

					Vertex v3 = {
						p3.x, p3.y, p3.z,


						1.0f,
						1.0f,
						1.0f,
						1.0f,

						0.0f,
						0.0f,
						-1.0f,

						0.0f,
						0.0f,

						texture_index
					};

					Vertex v4 = {
						p4.x, p4.y, p4.z,

						1.0f,
						1.0f,
						1.0f,
						1.0f,

						0.0f,
						0.0f,
						-1.0f,

						texco_u,
						0.0f,

						texture_index
					};

					buffer.push_back(v1);
					buffer.push_back(v2);
					buffer.push_back(v3);
					buffer.push_back(v4);
				}
				batch.element.vbo.Bind();
				glBufferSubData(GL_ARRAY_BUFFER, 0, buffer.size()*sizeof(Vertex), &buffer[0]);
				batch.element.vbo.Unbind();


			}

			auto& t = transforms[batch.id_transform].transformation;
			//if (batch.id_animator)
			//	t = animators[batch.id_animator].translation * t;
			render_graphic(batch, t, _camera, _light, _cam_pos, _dir); // transforms[0] is an 'identity' transformation
		}

		//LOG(LOGTYPE_WARNING, "Draw calls : ", std::to_string(draw_calls));
		draw_calls = 0;
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
