#ifndef GRAPHICS_3D
#define GRAPHICS_3D

#include <vector>
#include "Material.hpp"
#include "Component.hpp"
#include "Vertex.hpp"
#include "Transform3D.hpp"
#include "Constants.hpp"
#include <unordered_map>

#define MAX_VERT_ATTRIB 5

namespace PrEngine
{
    struct VertexArray
    {
        GLuint id;
        VertexArray();
        ~VertexArray();
		//VertexArray(const VertexArray& other) = delete;
		//VertexArray& operator=(const VertexArray& other) = delete;
		void Bind() const;
        void Unbind() const;
        void Generate();
        void Delete();
    };

    struct VertexBuffer
    {
        GLuint id;
        VertexBuffer();
        ~VertexBuffer();
		//VertexBuffer(const VertexBuffer& other) = delete;
		//VertexBuffer& operator=(const VertexBuffer& other) = delete;

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
		//IndexBuffer(const IndexBuffer& other) = delete;
		//IndexBuffer& operator=(const IndexBuffer& other) = delete;

        void Bind() const;
        void Unbind() const;
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

	struct GraphicEditorData // data only required for loading and saving
	{
		GraphicEditorData();
		GraphicEditorData(Float_32 scale, Int_32 future_tag);

		Float_32 scale;
		Int_32 future_tag;
	};

    struct Graphic : public Component	// 184 bytes
    {
		static std::unordered_map<Float_32, GraphicEditorData> editor_data;
		//static std::unordered_map < Uint_32, Vec3f[4]> vertex_data;
		static Vec3f vertex_data[Max_graphics_count][4];

		GraphicsElement element;			//156
		Vec3f outline_color;				//12
		Float_32 outline_alpha;				//4
		Uint_32 transform_id;				//4
		Uint_32 animator_id;				//4
		Uint_32 batch_id;					//4
		RenderTag tag;						//4


        Graphic();// const Vertex* vertices, GLuint vertices_size, const GLuint* indices, GLuint indices_size, GLsizei indices_count, Material material,Texture texture, VertexLayout layout);
        ~Graphic();
		//Graphic(const Graphic& other) = delete;
		//Graphic& operator=(const Graphic& other) = delete;
		void initialize() override;
		void start() override;
		void update() override;
		void end() override;
		void Delete();
        std::string to_string() override;
		

		//Rect bounding_rect;
	};


	struct Graphic_TexID
	{
		Uint_32 graphic_id;
		Uint_32 batched_diffuse_index;
	};

	struct BatchedGraphic : public Graphic
	{

		//static Uint_32 animation_to_batch_id[Max_animation_count];

		//std::unordered_map<Uint_32, Uint_32> texture_to_index;
		//Uint_32 texture_to_index[99999] = {};
		std::vector<Graphic_TexID> graphic_ids;

		static const Uint_32 max_vertices_in_batch = 4 * 10000; // 4 vertex per quad * 1000000 quads. each vertex 40 bytes.
		Uint_32 current_batched_vertex_count;
		Uint_32 current_batched_texture_count;
		
		static void initialize(Uint_32 batch_id, RenderTag render_mode);
		void initialize() override;
		void start() override;
		void update() override;
		void end() override;
		BatchedGraphic();
		//BatchedGraphic(const BatchedGraphic& other) = delete;
		//BatchedGraphic& operator=(const BatchedGraphic& other) = delete;
		~BatchedGraphic();

	private:
		Uint_32* indices;

	};

}


#endif
