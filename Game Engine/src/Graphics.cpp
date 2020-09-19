#include "Graphics.hpp"

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

    void VertexBuffer::Generate(const Vertex* vertices, GLuint size)
    {
        GL_CALL( 
            glGenBuffers(1, &id))
        Bind(); 
        GL_CALL( 
            glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW))
        
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

    Graphics::Graphics(const Matrix4x4<float>& model, const Matrix4x4<float>& normal):model(model),normal(normal),Component(COMP_GRAPHICS)
    {

    }

    Graphics::~Graphics()
    {
        for(std::vector<GraphicsElement>::iterator it = elements.begin(); it!= elements.end(); it++)
            it->Delete();
        
        //delete material;
    }

    std::string Graphics::to_string()
    {
    	return std::to_string(COMP_GRAPHICS);
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

    VertexAttribute::VertexAttribute(GLuint index, GLuint count, int type, GLboolean normalized)
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
        if(vertex_attributes.size() >= 1)
        {
            attribute.offset = (vertex_attributes.back().size + vertex_attributes.back().offset);
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
        vertex_attributes.push_back(attribute);

    }
}
