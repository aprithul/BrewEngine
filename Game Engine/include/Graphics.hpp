#ifndef GRAPHICS_3D
#define GRAPHICS_3D

#include <vector>
#include <GL/glew.h>
#include "Material.hpp"
#include "Component.hpp"
#include "Vertex.hpp"
#include "Matrix4x4f.hpp"
#include "Transform3D.hpp"


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
        void Generate(const Vertex* vertices, GLuint size, Uint_32 usage);
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
            Int_32 type;
            GLboolean normalized;
            GLsizei offset;

            GLsizei size;

            VertexAttribute(GLuint index, GLuint count, Int_32 type, GLboolean normalized);
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
		VertexLayout layout;
        VertexArray vao;
        VertexBuffer vbo;
        IndexBuffer ibo;
        Uint_32 material;
        Int_32 num_of_triangles;
        void Delete();
    };

	enum RenderTag
	{
		RENDER_UNTAGGED,
		RENDER_STATIC,
		RENDER_DYNAMIC,

		RENDER_TAG_COUNT
	};

    struct Graphic : public Component
    {
		GraphicsElement element;
		Vector3<Float_32> outline_color;
		Uint_32 id_transform;
		Float_32 outline_alpha;
		Uint_32 id_animator;
		RenderTag tag;


        Graphic();// const Vertex* vertices, GLuint vertices_size, const GLuint* indices, GLuint indices_size, GLsizei indices_count, Material material,Texture texture, VertexLayout layout);
        ~Graphic();
		void start() override;
        std::string to_string() override;
		

		//Rect bounding_rect;
	};


	struct BatchedGraphic : public Graphic
	{
		static const Uint_32 max_vertices_in_batch = 4*1000; // 4 vertex per quad * 1000 quads. Arbitrarily set.
		static const Uint_32 max_textures_in_batch = MAX_TEXTURES;

		static Uint_32 current_batched_vertex_count;
		static Uint_32 current_batched_texture_count;
		std::unordered_map<Uint_32, Uint_32> texture_to_index;
		std::vector<Uint_32> graphic_ids;

		BatchedGraphic(int batch_index);
		~BatchedGraphic();
	};
}


#endif
