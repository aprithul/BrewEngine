#include "ShapesLayer.hpp"
#include "RendererOpenGL2D.hpp"

namespace PrEngine {

	void ShapesLayer::start()
	{
	}

	void ShapesLayer::update()
	{

		Uint_32 camera_id = entity_management_system->get_active_camera();
		if (!camera_id)
			return;

		Camera& _camera = cameras[camera_id];

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

			GLint* uniform_loc = Shader::shader_library[mat->shader].uniform_locations;


			for (Uint_32 _i = 0; _i < (Int_32)ShaderUniformName::u_count; _i++)
			{
				ShaderUniformName _name = (ShaderUniformName)_i;
				GLuint _loc = uniform_loc[_i];
				if (_loc >= 0)
				{
					switch (_name)
					{
						/*case ShaderUniformName::u_Model:
							GL_CALL(
								glUniformMatrix4fv(_loc, 1, GL_TRUE, Matrix4x4<Float_32>::identity().data);
							)
								break;*/
					case ShaderUniformName::u_View:
						GL_CALL(
							glUniformMatrix4fv(_loc, 1, GL_TRUE, _camera.view_matrix.data);
						)
							break;
					case ShaderUniformName::u_Projection:
						GL_CALL(
							glUniformMatrix4fv(_loc, 1, GL_TRUE, _camera.projection_matrix.data);
						)
							break;
					default:
						break;
					}
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
	}

	void ShapesLayer::end()
	{
	}

}