#include "Graphics.hpp"
//#include "Transform3D.hpp"
#include "EntityManagementSystemModule.hpp"

namespace PrEngine
{
    VertexArray::VertexArray()
    {
    }

    VertexArray::~VertexArray()
    {
    }
    
    void VertexArray::Generate()
    {
		GL_CALL(
			glGenVertexArrays(1, &id))
			LOG(LOGTYPE_WARNING, "Created vao: " + std::to_string(id));
        Bind();
    }

    void VertexArray::Delete()
    {
        GL_CALL(
            glDeleteVertexArrays(1, &id))
    }

    void VertexArray::Bind() const
    {
        GL_CALL(
            glBindVertexArray(id))
    }

    void VertexArray::Unbind() const
    {
        GL_CALL(
            glBindVertexArray(0))
    }

    VertexBuffer::VertexBuffer()
    {
    }

    void VertexBuffer::Generate(const Vertex* vertices, GLuint size, Uint_32 usage)
    {
        GL_CALL( 
            glGenBuffers(1, &id))
		LOG(LOGTYPE_WARNING, "Created vbo: " + std::to_string(id));

        Bind(); 
		glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);//)

    }

    VertexBuffer::~VertexBuffer()
    {
    }

    void VertexBuffer::Delete()
    {
        GL_CALL( 
            glDeleteBuffers(1, &id));
    }

    void VertexBuffer::Bind()
    {
        GL_CALL(
            glBindBuffer(GL_ARRAY_BUFFER, id));
    }

    void VertexBuffer::Unbind()
    {
        GL_CALL(
            glBindBuffer(GL_ARRAY_BUFFER, 0));
		return;
    }


    IndexBuffer::IndexBuffer()
    {
    }

    void IndexBuffer::Generate(const GLuint* indices, GLuint indices_size, GLsizei count)
    {
        this->count = count;
        GL_CALL( 
            glGenBuffers(1, &id));
        Bind();
        GL_CALL(
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW))
    }

    IndexBuffer::~IndexBuffer()
    {

    }

    void IndexBuffer::Delete()
    {
        GL_CALL( 
            glDeleteBuffers(1, &id));
    }

    void IndexBuffer::Bind() const
    {
        GL_CALL(
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    }

    void IndexBuffer::Unbind() const
    {
        GL_CALL(
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

	GraphicEditorData::GraphicEditorData()
	{
		scale = 1.f;
		future_tag = RENDER_DYNAMIC;
	}

	GraphicEditorData::GraphicEditorData(Float_32 _scale, Int_32 _future_tag)
	{
		scale = _scale;
		future_tag = _future_tag;
	}

	std::unordered_map<Float_32, GraphicEditorData> Graphic::editor_data;
	//std::unordered_map < Uint_32, Vec3f[4]> Graphic::vertex_data;
	Vec3f Graphic::vertex_data[Max_graphics_count][4];

    Graphic::Graphic():Component(COMP_GRAPHICS)
    {
		outline_color = Vec3f{ 1,1,1 };
		outline_alpha = 0.f;
		animator_id = 0;
		transform_id = 0;
		batch_id = 0;
    }

	void Graphic::start()
	{
	}

    Graphic::~Graphic()
    {
		
		LOG(LOGTYPE_GENERAL, "deleted graphic");
        element.Delete();
        //delete material;
    }

	void Graphic::Delete()
	{
		element.Delete();
	}

	//Uint_32 BatchedGraphic::current_batched_vertex_count = 0;
	//Uint_32 BatchedGraphic::current_batched_texture_count = 0;

	//Uint_32 BatchedGraphic::animation_to_batch_id[Max_animation_count] = {};

	BatchedGraphic::BatchedGraphic():current_batched_texture_count(0), current_batched_vertex_count(0)
	{
	}

	BatchedGraphic::~BatchedGraphic()
	{
		delete[] indices;
	}

	static const Uint_32 ind_len = (BatchedGraphic::max_vertices_in_batch / 4) * 6;
	void BatchedGraphic::initialize(Uint_32 batch_id, RenderTag render_mode)
	{
		
		BatchedGraphic& batch = batched_graphics_system.get_component(batch_id);
		batch.indices = new Uint_32[ind_len];

		for (Uint_32 index = 0, pos = 0; pos < ind_len; index+=4, pos += 6)
		{
			batch.indices[pos] = index;
			batch.indices[pos + 1] = index + 1;
			batch.indices[pos + 2] = index + 2;
			batch.indices[pos + 3] = index + 2;
			batch.indices[pos + 4] = index + 3;
			batch.indices[pos + 5] = index;
		}

		VertexLayout layout;
		VertexAttribute attribute_0(0, 3, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_1(1, 4, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_2(2, 2, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_3(3, 1, GL_FLOAT, GL_FALSE);

		layout.add_attribute(attribute_0);
		layout.add_attribute(attribute_1);
		layout.add_attribute(attribute_2);
		layout.add_attribute(attribute_3);

		batch.element.material = Material::load_material("Materials" + PATH_SEP + "Batch.mat", false, std::to_string(batch_id));
		batch.transform_id = 0;

		batch.element.vao.Generate();
		batch.element.vbo.Generate(nullptr, sizeof(Vertex) * BatchedGraphic::max_vertices_in_batch, 
																		render_mode==RENDER_DYNAMIC? GL_STREAM_DRAW : GL_STATIC_DRAW);
		batch.element.layout = layout;

		for (Uint_32 _i = 0; _i < batch.element.layout.next_attrib; _i++)// != batch.element.layout.vertex_attributes.end(); attr++)
		{
			VertexAttribute* attr = &(batch.element.layout.vertex_attributes[_i]);
			GL_CALL(
				glEnableVertexAttribArray(attr->index))
				//GL_CALL(
				glVertexAttribPointer(attr->index, attr->count, attr->type, attr->normalized, layout.stride, (void*)attr->offset);//)
		}

		batch.element.ibo.Generate(&batch.indices[0], ind_len * sizeof(GLuint), ind_len);
		batch.element.vao.Unbind();
		batch.element.ibo.Unbind();
	}

    std::string Graphic::to_string()
    {
		//Material* mat = Material::get_material(this->element.material);
		return std::to_string(COMP_GRAPHICS) + "," + Material::material_names[element.material];// +"," + std::to_string(future_tag);// Texture::texture_names[mat->diffuse_texture];
    }

    void GraphicsElement::Delete()
    {
        vao.Delete();
        vbo.Delete();
        ibo.Delete();
        //material->Delete();
        //delete material;
    }

    VertexLayout::VertexLayout()
    {
        stride = sizeof(Vertex);
    }

	VertexAttribute::VertexAttribute()
	{
		this->index = 0;
		this->count = 0;
		this->type = 0;
		this->normalized = 0;
		this->offset = 0;
		this->size = 0;
	}

    VertexAttribute::VertexAttribute(GLuint index, GLuint count, Int_32 type, GLboolean normalized)
    {
        this->index = index;
        this->count = count;
        this->type  = type;
        this->normalized = normalized;
        this->offset = 0;
        this->size = 0;
    }

    void VertexLayout::add_attribute(VertexAttribute& attribute)
    {
        if(next_attrib >= 1)
        {
            attribute.offset = (vertex_attributes[next_attrib-1].size + vertex_attributes[next_attrib-1].offset);
        }

        switch (attribute.type)
        {
        case GL_FLOAT:
            attribute.size = (sizeof(GLfloat) * attribute.count);
            break;
        case GL_UNSIGNED_INT:
            attribute.size = (sizeof(GLuint) * attribute.count);
            break;
        case GL_INT:
            attribute.size = (sizeof(GLint) * attribute.count);
            break;
            //....
            //....
            //....
        default:
            LOG(LOGTYPE_ERROR, "Attribute type not found");
            break;
        }

        //stride += attribute.size;
        vertex_attributes[next_attrib]=attribute;
		next_attrib++;
    }
}