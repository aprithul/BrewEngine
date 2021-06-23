#include "ShapesLayer.hpp"
#include "RendererOpenGL2D.hpp"
#include <time.h>
#define MAX_LINES 2000

namespace PrEngine {
	std::vector<Line> lines;
	std::vector<GLuint> lines_indices;
	std::vector<Vertex> lines_buffer;
	Graphic line_graphic;

	void make_shape_graphic()
	{
		Uint_32 mat_id = Material::load_material("Materials" + PATH_SEP + "shape.mat", true);
		Material& mat = Material::material_library[mat_id];
		line_graphic.element.material = mat_id;


		for (int i = 0; i < MAX_LINES; i++)
			lines_indices.push_back(i);

		VertexLayout layout;
		VertexAttribute attribute_0(0, 3, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_1(1, 4, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_2(2, 3, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_3(3, 2, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_4(4, 1, GL_FLOAT, GL_FALSE);
		layout.add_attribute(attribute_0);
		layout.add_attribute(attribute_1);
		layout.add_attribute(attribute_2);
		layout.add_attribute(attribute_3);
		layout.add_attribute(attribute_4);

		line_graphic.element.vao.Generate();
		line_graphic.element.vbo.Generate(nullptr, sizeof(Vertex)*MAX_LINES *2, GL_DYNAMIC_DRAW);
		line_graphic.element.layout = layout;

		for (auto attr : line_graphic.element.layout.vertex_attributes)
		{
			GL_CALL(
				glEnableVertexAttribArray(attr.index))
				GL_CALL(
					glVertexAttribPointer(attr.index, attr.count, attr.type, attr.normalized, layout.stride, (void*)attr.offset))
		}

		line_graphic.element.ibo.Generate(&lines_indices[0], lines_indices.size() * sizeof(GLuint), lines_indices.size());
	}

	void ShapesLayer::start()
	{
		make_shape_graphic();
	}

	void ShapesLayer::update()
	{
		//clock_t begin = clock();
		Uint_32 camera_id = entity_management_system->get_active_camera();
		if (!camera_id)
			return;
		Camera& _camera = cameras[camera_id];

		GraphicsElement& element = line_graphic.element;

		for (int _i = 0; _i < lines.size(); _i++)
		{	
			Vertex v1 = { lines[_i].p1.x , lines[_i].p1.y, -10,  lines[_i].color.x, lines[_i].color.y, lines[_i].color.z, lines[_i].color.w, 0,0, 0 };
			Vertex v2 = { lines[_i].p2.x , lines[_i].p2.y, -10,  lines[_i].color.x, lines[_i].color.y, lines[_i].color.z, lines[_i].color.w, 0,0, 0 };
			lines_buffer.push_back(v1);
			lines_buffer.push_back(v2);
		}

		element.vbo.Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, lines_buffer.size() * sizeof(Vertex), &lines_buffer[0]);
		element.vbo.Unbind();


		if (lines_buffer.size() > 0)
		{
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
					case ShaderUniformName::u_View:
						GL_CALL(
							glUniformMatrix4fv(_loc, 1, GL_FALSE, _camera.view_matrix.data);
						)
							break;
					case ShaderUniformName::u_Projection:
						GL_CALL(
							glUniformMatrix4fv(_loc, 1, GL_FALSE, _camera.projection_matrix.data);
						)
							break;
					default:
						break;
					}
				}
			}

			GL_CALL(
				glDrawElements(GL_LINES, lines.size()*2, GL_UNSIGNED_INT, nullptr));

			element.vao.Unbind();
			element.ibo.Unbind();
			mat->Unbind();

			lines_buffer.clear();
			lines.clear();

		}

		/*clock_t end = clock();
		Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;
		return;
*/

	}

	void ShapesLayer::end()
	{
	}

}