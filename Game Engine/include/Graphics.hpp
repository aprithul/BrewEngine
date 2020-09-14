#ifndef GRAPHICS_3D
#define GRAPHICS_3D

#include <vector>
#include <GL/glew.h>
#include "Material.hpp"
#include "Component.hpp"
#include "Vertex.hpp"
#include "Matrix4x4f.hpp"

namespace PrEngine
{
    struct VertexArray
    {
        GLuint id;
        VertexArray();
        ~VertexArray();
        void Bind();
        void Unbind();
        void Generate();
        void Delete();
    };

    struct VertexBuffer
    {
        GLuint id;
        VertexBuffer();
        ~VertexBuffer();
        void Bind();
        void Unbind();
        void Generate(const Vertex* vertices, GLuint size);
        void Delete();
    };

    struct IndexBuffer
    {
        GLuint id;
        GLsizei count;
        IndexBuffer();
        ~IndexBuffer();
        void Bind();
        void Unbind();
        void Generate(const GLuint* indices, GLuint indices_size, GLsizei count);
        void Delete();
    };

    struct VertexAttribute
        {
            GLuint index;     
            GLuint count;
            int type;
            GLboolean normalized;
            GLsizei offset;

            GLsizei size;

            VertexAttribute(GLuint index, GLuint count, int type, GLboolean normalized);
    };

    struct VertexLayout
    {
        VertexLayout();
        GLsizei stride;
        std::vector<VertexAttribute> vertex_attributes;
        void add_attribute(VertexAttribute& attribute);
    };

    struct GraphicsElement
    {
        VertexArray vao;
        VertexBuffer vbo;
        IndexBuffer ibo;
        Material* material;
        VertexLayout layout;
        int num_of_triangles;
        void Delete();
    };

    struct Graphics : public Component
    {
        std::vector<GraphicsElement> elements;
        //Material* material;
        //VertexLayout layout;  

        const Matrix4x4<float>* model;
        const Matrix4x4<float>* normal;

        Graphics();// const Vertex* vertices, GLuint vertices_size, const GLuint* indices, GLuint indices_size, GLsizei indices_count, Material material,Texture texture, VertexLayout layout);
        ~Graphics();
    };
}


#endif