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

    void VertexArray::Bind()
    {
        GL_CALL(
            glBindVertexArray(id))
    }

    void VertexArray::Unbind()
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

    void IndexBuffer::Bind()
    {
        GL_CALL(
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    }

    void IndexBuffer::Unbind()
    {
        GL_CALL(
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

	std::unordered_map<Uint_32, GraphicEditorData> Graphic::editor_data;
	std::unordered_map < Uint_32, Vec3f[4]> Graphic::vertex_data;


    Graphic::Graphic():Component(COMP_GRAPHICS)
    {
		outline_color = Vec3f{ 1,1,1 };
		outline_alpha = 0.f;
		animator_id = 0;
		transform_id = 0;
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

	Uint_32 BatchedGraphic::current_batched_vertex_count = 0;
	Uint_32 BatchedGraphic::current_batched_texture_count = 0;

	BatchedGraphic::BatchedGraphic()
	{
	}

	BatchedGraphic::~BatchedGraphic()
	{

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