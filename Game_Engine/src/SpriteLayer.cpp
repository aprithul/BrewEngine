#include "SpriteLayer.hpp"
#include "RendererOpenGL2D.hpp"
#include <time.h>
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
	//inline void render_graphic(Graphic& graphic, Mat4x4& transformation, Camera& _camera, DirectionalLight& _light, Vec3f _cam_pos, Vec3f _dir)
	//{
	//	
	//	draw_calls++;
	//	GraphicsElement& element = graphic.element;
	//	Material* mat = Material::get_material(element.material);
	//	if (mat == nullptr)
	//	{
	//		LOG(LOGTYPE_ERROR, "Couldn't find material");
	//		return;
	//	}

	//	//graphic.bounding_rect.x = transform.position.x;
	//	//graphic.bounding_rect.y = transform.position.y;
	//	//std::cout<<"before: "<<grp->element.material.uniform_locations["u_MVP"]  <<std::endl;

	//	// only bind texture for the last texture unit. Ideally we want to have fewer units in use
	//	// to avoid binding all together. Assumption is that textures are bound at consturction and
	//	// remain bound
	//	Texture* tex = Texture::get_texture(mat->diffuse_textures[0]);
	//	//if (tex->bind_unit == max_texture_units - 1)
	//	mat->Bind();

	//	//glActiveTexture(GL_TEXTURE0 + tex->bind_unit);
	//	Shader* shader = Shader::get_shader(mat->shader);
	//	if (shader == nullptr)
	//	{
	//		LOG(LOGTYPE_ERROR, "Shader couldn't be found");
	//		return;
	//	}
	//	//glUseProgram(shader->id);
	//	GLint* uniform_loc = shader->uniform_locations;


	//	for (Uint_32 _i = 0; _i<(Int_32)ShaderUniformName::u_count; _i++)
	//	{
	//		ShaderUniformName _name = (ShaderUniformName)_i;
	//		GLint _loc = uniform_loc[_i];
	//		if (_loc >= 0)
	//		{
	//			switch (_name)
	//			{
	//				case ShaderUniformName::u_sampler2d:
	//				{
	//					Texture* tex = Texture::get_texture(mat->diffuse_textures[0]);
	//					//GL_CALL(
	//					glUniform1i(_loc, tex->bind_unit);//)
	//				}
	//				break;
	//				case ShaderUniformName::u_textures:	//happens with batche graphic, batche graphi will have array of texture ids
	//				{
	//					/*GLint texture_bind_units[MAX_TEXTURES] = {};
	//					int _count = 0;
	//					for (int _i = 0; _i < MAX_TEXTURES; _i++)
	//					{
	//						//if (mat->diffuse_textures[_i] != 0)
	//						{
	//							Texture* tex = Texture::get_texture(mat->diffuse_textures[_i]);
	//							texture_bind_units[_i] = tex->bind_unit;
	//							_count++;
	//						}
	//						//else
	//						//	break;
	//					}

	//					//GL_CALL(
	//					glUniform1iv(it.second.second, _count, texture_bind_units);//)*/
	//				}
	//				break;
	//				/*case ShaderUniformName::u_Dir_Light:
	//					GL_CALL(
	//						glUniform3f(it.second.second, _dir.x, _dir.y, _dir.z))
	//						break;*/
	//				case ShaderUniformName::u_Model:
	//					GL_CALL(
	//						glUniformMatrix4fv(_loc, 1, GL_FALSE, transformation.data))
	//						break;
	//				case ShaderUniformName::u_View:
	//					GL_CALL(
	//						glUniformMatrix4fv(_loc, 1, GL_FALSE, _camera.view_matrix.data))
	//						break;
	//				case ShaderUniformName::u_View_t:
	//				{
	//					Mat4x4 _view = Mat4x4(_camera.view_matrix);
	//					_view(0,3) = 0;
	//					_view(1,3) = 0;
	//					_view(2,3) = 0;
	//					_view(3,0) = 0;
	//					_view(3,1) = 0;
	//					_view(3,2) = 0;
	//					_view(3,3) = 1;

	//					GL_CALL(
	//						glUniformMatrix4fv(_loc, 1, GL_FALSE, _view.data))
	//				}
	//				break;
	//				case ShaderUniformName::u_Projection:
	//					GL_CALL(
	//						glUniformMatrix4fv(_loc, 1, GL_FALSE, _camera.projection_matrix.data))
	//						break;
	//					/*case ShaderUniformName::u_Camera_Position:
	//						GL_CALL(
	//							glUniform3f(it.second.second, _cam_pos.x, _cam_pos.y, _cam_pos.z))
	//							break;
	//					case ShaderUniformName::u_Normal_M:
	//						GL_CALL(
	//							glUniformMatrix4fv(it.second.second, 1, GL_TRUE, transform.rotation_transformation.data))
	//							break;*/
	//				case ShaderUniformName::u_Panning:
	//					GL_CALL(
	//						glUniform2f(_loc, mat->panning.x, mat->panning.y))
	//						break;
	//				case ShaderUniformName::u_Tiling:
	//					GL_CALL(
	//						glUniform2f(_loc, mat->tiling.x, mat->tiling.y))
	//						break;
	//				case ShaderUniformName::u_Diffuse_Color:
	//					GL_CALL(
	//						glUniform3f(_loc, mat->diffuse_color.x, mat->diffuse_color.y, mat->diffuse_color.z))
	//						break;
	//				case ShaderUniformName::u_Outline_Color:
	//					GL_CALL(
	//						glUniform4f(_loc, graphic.outline_color.x, graphic.outline_color.y, graphic.outline_color.z, graphic.outline_alpha))
	//						break;
	//				case ShaderUniformName::u_Ambient_Strength:
	//					GL_CALL(
	//						glUniform1f(_loc, 1.f))
	//						break;
	//				default:
	//					break;
	//			}
	//		}



	//		//}

	//		//grp->ibo[i].Bind();

	//		//GL_CALL(
	//		//    glUniform1f((*it)->material.uniform_locations["u_red"], 1.f))
	//	}
	//	element.vao.Bind();
	//	element.ibo.Bind();
	//	GL_CALL(
	//		//glDrawArrays(GL_TRIANGLES,0, grp->element.num_of_triangles*3))
	//		glDrawElements(GL_TRIANGLES, element.ibo.count, GL_UNSIGNED_INT, nullptr));
	//	element.vao.Unbind();
	//	element.ibo.Unbind();
	//	mat->Unbind();

	//}

	//Float_32 texture_index = 0;
	std::vector<Uint_32> dynamic_batch_graphic_ids;
	Bool_8 static_batches_prepared = false;
	void SpriteLayer::update()
	{

		static Bool_8 generated_static_batches = false;

		//insertion_sort(sprite_list, sprite_list.size());
		Uint_32 camera_id = entity_management_system->get_active_camera();
		if (!camera_id)
			return;

		Camera& _camera = camera_system.get_component(camera_id);
		//DirectionalLight& _light = directional_lights[1];
		//Vec3f _cam_pos = transforms[_camera.id_transform].position;
		//Vec3f _dir = get_transform(_light.id_transform).get_forward();

		dynamic_batch_graphic_ids.clear();
		//const Graphic* components = graphics_system.get_comp_array();
		for (Uint_32 _i = 1; _i < graphics_system.new_id; _i++)
		{

			if (graphics_system.get_entity(_i))//  is_valid(graphic_active_status, graphics[_i].entity))
			{
				//UpdateTransforms(transform);
				//Matrix4x4<Float_32> mvp = (projection) * (*(grp->model)) ;
				//const auto& graphic = components[_i];
				const auto& graphic = graphics_system.get_component(_i);
				if (graphic.tag == RENDER_UNTAGGED)
				{
					auto& transform = transform_system.get_component(graphic.transform_id);
					renderer->render_graphic(graphic, 6, transform.transformation, _camera);
				}
				else if (graphic.tag == RENDER_DYNAMIC || !static_batches_prepared)
				{
					renderer->add_to_batch(_i, graphic.tag);
				}
				//else if (graphic.tag == RENDER_DYNAMIC)
				//{
				//	if(!graphic.batch_id)	//may not contain the correct batch id (since animations can be in different batches)
				//		dynamic_batch_graphic_ids.push_back(_i);
				//	//renderer->add_to_batch(_i);
				//}
			}
		}

		/*if (dynamic_batch_graphic_ids.size() > 0)
		{
			renderer->add_to_batch(dynamic_batch_graphic_ids);
			dynamic_batch_graphic_ids.clear();
		}*/



		//renderer->prepare_batches(dynamic_batch_graphic_ids, GL_STREAM_DRAW);


		clock_t begin = clock();

		for (Uint_32 _i =1; _i < batched_graphics_system.new_pos; _i++)
		{
			//BatchedGraphic& batch = batched_graphics[_i];
			BatchedGraphic& batch = batched_graphics_system.get_component(_i);
			if (batch.tag == RENDER_DYNAMIC || !static_batches_prepared)
			{
				std::vector<Vertex> buffer;

				for (int _i=0; _i<batch.graphic_ids.size(); _i++)
				{
					Graphic_TexID graphic_texid = batch.graphic_ids[_i];
					Uint_32 g_id = graphic_texid.graphic_id;
					

					Graphic& graphic = graphics_system.get_component(g_id);
					Uint_32 material_id = graphic.element.material;
					assert(material_id);
					Material* mat = Material::get_material(material_id);

					Uint_32 animator_id = graphic.animator_id;
					Uint_32 texture_id = 0;
					//Mat4x4 anim_tr = Mat4x4::Identity();
					texture_id = mat->diffuse_textures[0];

					if (animator_id)
					{
						//Animator& anim = animators[animator_id];
						Animator& anim = animator_system.get_component(animator_id);
						Animation& animation = anim.get_current_animation();
						if (animation.frames.size() > anim.current_frame_index)
						{
							//anim_tr = anim.translation*anim.rotation.GetRotationMatrix()*anim.scale;
							texture_id = animation.frames[anim.current_frame_index].texture;
						}
					}

					//first get the index between [0,max_layers). Then add the tex location in batch diffuse tex array to it
					// to get the final 'encoded' texture index
					Float_32 texture_index = renderer->is_included_at_index[texture_id] % renderer->max_layers;
					texture_index = (graphic_texid.batched_diffuse_index * renderer->max_layers + texture_index);


					//texture_index = batch.texture_to_index[texture_id];
					//LOG(LOGTYPE_WARNING, std::to_string(texture_index));
					Texture* tex = Texture::get_texture(texture_id); //Material::material_library[mat_id].diffuse_texture;
					Float_32 x_scale = tex->width;
					Float_32 y_scale = tex->height;
					Float_32 texco_u = clamp(tex->width / (Float_32)MAX_TEXTURE_SIZE, 0.1f, 1.0f);
					Float_32 texco_v = clamp(tex->height / (Float_32)MAX_TEXTURE_SIZE, 0.1f, 1.0f);

					if (x_scale >= y_scale)
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

					Transform3D& transform = transform_system.get_component(graphic.transform_id);
					Vec3f p1 = transform.transformation * Vec3f{ 0.5f*x_scale, 0.5f*y_scale, 0.0f };
					Vec3f p2 = transform.transformation * Vec3f{ -0.5f*x_scale, 0.5f*y_scale, 0.0f };
					Vec3f p3 = transform.transformation * Vec3f{ -0.5f*x_scale, -0.5f*y_scale, 0.0f };
					Vec3f p4 = transform.transformation * Vec3f{ 0.5f*x_scale, -0.5f*y_scale, 0.0f };


					//if ( !static_batches_prepared)
					{
						Graphic::vertex_data[g_id][0] = Vec3f{ 0.5f*x_scale, 0.5f*y_scale, 0.0f };
						Graphic::vertex_data[g_id][1] = Vec3f{ -0.5f*x_scale, 0.5f*y_scale, 0.0f };
						Graphic::vertex_data[g_id][2] = Vec3f{ -0.5f*x_scale, -0.5f*y_scale, 0.0f };
						Graphic::vertex_data[g_id][3] = Vec3f{ 0.5f*x_scale, -0.5f*y_scale, 0.0f };
					}
					
					


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

						texture_index
					};

					Vertex v4 = {
						p4.x, p4.y, p4.z,

						1.0f,
						1.0f,
						1.0f,
						1.0f,

						texco_u,
						0.0f,

						texture_index
					};


					{	// save vertex data for collider
						
					}

					buffer.push_back(v1);
					buffer.push_back(v2);
					buffer.push_back(v3);
					buffer.push_back(v4);
				}
				if (buffer.size() > 0)
				{
					/*if (buffer.size() == 4)
					{
						buffer.push_back({ 0,0,0,0,0,0,0,0,0,0 });
						buffer.push_back({ 0,0,0,0,0,0,0,0,0,0 });
						buffer.push_back({ 0,0,0,0,0,0,0,0,0,0 });
						buffer.push_back({ 0,0,0,0,0,0,0,0,0,0 });
					}*/
					batch.element.vbo.Bind();
					glBufferSubData(GL_ARRAY_BUFFER, 0, buffer.size() * sizeof(Vertex), &buffer[0]);
					//glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(Vertex), &buffer[0], GL_STREAM_DRAW);
					batch.element.vbo.Unbind();
				}
				draw_calls = 0;
			}
			
			
			if (batch.graphic_ids.size() > 0)
			{
				Mat4x4& _transformation = transform_system.get_component(batch.transform_id).transformation;
				//if (batch.id_animator)
				//	t = animators[batch.id_animator].translation * t;
				renderer->render_graphic(batch, batch.graphic_ids.size() * 6, _transformation, _camera); // transforms[0] is an 'identity' transformation
			}
			
		}
				//LOG(LOGTYPE_WARNING, "Draw calls : ", std::to_string(draw_calls));

		static_batches_prepared = true;

		clock_t end = clock();

		Float_32 seconds = (float)(end - begin) / CLOCKS_PER_SEC;
		//LOG(LOGTYPE_GENERAL, std::to_string(seconds));
		
		return;
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
