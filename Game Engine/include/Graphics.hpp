#ifndef GRAPHICS_3D
#define GRAPHICS_3D

#include <vector>
#include <GL/glew.h>
#include "Material.hpp"
#include "Component.hpp"
#include "Vertex.hpp"
#include "Matrix4x4f.hpp"
#include "Transform3D.hpp"

#define MAX_VERT_ATTRIB 5

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

    struct VertexAttribute // 24 bytes
        {
            GLuint index;     
            GLuint count;
            Int_32 type;
            GLsizei offset;
            GLsizei size;
			GLboolean normalized;

			VertexAttribute();
            VertexAttribute(GLuint index, GLuint count, Int_32 type, GLboolean normalized);
    };

    struct VertexLayout	// 128 bytes
    {
		VertexAttribute vertex_attributes[MAX_VERT_ATTRIB]; // 120 bytes   24*MAX_VERT_ATTRIB
		GLsizei stride;
		Uint_32 next_attrib = 0;
		VertexLayout();
        void add_attribute(VertexAttribute& attribute);
    };

    struct GraphicsElement	//152 bytes
    {
		VertexLayout layout;	// 128
		IndexBuffer ibo;		// 8
		VertexArray vao;		// 4
        VertexBuffer vbo;		// 4
        Uint_32 material;		// 4
        Int_32 num_of_triangles;// 4
        void Delete();
    };

	enum RenderTag
	{
		RENDER_UNTAGGED,
		RENDER_STATIC,
		RENDER_DYNAMIC,

		RENDER_TAG_COUNT
	};

    struct Graphic : public Component	// 184 bytes
    {
		GraphicsElement element;			//152
		Vector3<Float_32> outline_color;	//12
		Float_32 outline_alpha;				//4
		Uint_32 id_transform;				//4
		Uint_32 id_animator;				//4
		Int_32 future_tag;					//4
		RenderTag tag;						//4


        Graphic();// const Vertex* vertices, GLuint vertices_size, const GLuint* indices, GLuint indices_size, GLsizei indices_count, Material material,Texture texture, VertexLayout layout);
        ~Graphic();
		void start() override;
        std::string to_string() override;
		

		//Rect bounding_rect;
	};


	struct BatchedGraphic : public Graphic
	{
		std::unordered_map<Uint_32, Uint_32> texture_to_index;
		std::vector<Uint_32> graphic_ids;

		static const Uint_32 max_vertices_in_batch = 4 * 1000; // 4 vertex per quad * 1000 quads. Arbitrarily set.
		static const Uint_32 max_textures_in_batch = MAX_TEXTURES;
		static Uint_32 current_batched_vertex_count;
		static Uint_32 current_batched_texture_count;
		
		BatchedGraphic();
		~BatchedGraphic();
	};
}


#endif
