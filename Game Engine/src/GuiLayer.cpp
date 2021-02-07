#include "GuiLayer.hpp"
#include "Logger.hpp"
#include "EntityManagementSystemModule.hpp"
#include "RendererOpenGL2D.hpp"
#include <queue>
#include "EntityGenerator.hpp"
namespace PrEngine
{
	Uint_32 selected_transform = 0;
	Uint_32 last_selected_transform = 0;
    GuiLayer::GuiLayer(SDL_Window* sdl_window, SDL_GLContext* gl_context):window(sdl_window),gl_context(gl_context)
    {
        this->name = "GUI";
        panning = nullptr;
        tiling = nullptr;
		this->fps = 0;
    }

    GuiLayer::~GuiLayer()
    {
        
    }

    void GuiLayer::start()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init("#version 410");
        
        //inspector initialization
        inspector_active = true;
        //ImGui::StyleColorsClassic();
    }

	Uint_32 mouse_pointer_transform = 0;

    void GuiLayer::update()
    {
        IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!"); // Exceptionally add an extra assert here for people confused with initial dear imgui setup
        
        ImGuiIO& io = ImGui::GetIO();
        //io.DisplaySize = ImVec2( 1440, 900);
        //io.DisplayFramebufferScale
        io.DeltaTime = Time::Frame_time;
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        static Bool_8 show = true;
        //ImGui::ShowDemoWindow(&show);

		Uint_32 cam = entity_management_system->get_active_camera();

		/*selected_transform = 0;
		for (Uint_32 _i = 0; _i < MAX_GRAPHIC_COUNT; _i++)
		{
			if (entity_management_system->graphics_entity_id[_i])
			{
				auto& g = graphics[_i];
				auto& g_t = transforms[g.id_transform];
				if (inside(cameras[cam].get_screen_to_world_pos(input_manager->mouse.position), g.bounding_rect, true))
				{
					selected_transform = g.id_transform;
				}

			}
		}*/
		if (input_manager->keyboard.get_key_down(SDLK_g))
			entity_management_system->delete_entity_transform(selected_transform);

		if (input_manager->keyboard.get_key_down(SDLK_l))
			renderer->draw_line(Vector2<Float_32>{0, 0}, Vector2<Float_32>{5, 0});

		if (input_manager->keyboard.get_key_down(SDLK_c))
		{
			std::string image_file_path = "braid\\door3.png";
			EntityGenerator eg;
			auto e = eg.make_graphics_entity(image_file_path);
			auto t_id = entities[e][COMP_TRANSFORM_3D];
			auto& pos = transforms[t_id].position;
			pos.x = rand() % 4* (rand() % 2 ? 1 : -1);
			pos.y = rand() % 2* (rand() % 2 ? 1 : -1);

			mouse_pointer_transform = t_id;
		}

		if (mouse_pointer_transform)
		{
			transforms[mouse_pointer_transform].position = cameras[cam].get_screen_to_world_pos(input_manager->mouse.position);
		}

		draw_editor();
		ImGui::ShowMetricsWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData());
    }

    void GuiLayer::end()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
    }
	
	//std::queue<Uint_32> transform_queue;
	//void add_to_hierarchy()
	//{

	//	while (!transform_queue.empty())
	//	{
	//		char buffer[64];
	//		sprintf_s(buffer, "%d", transform_queue.front());
	//		transform_queue.pop();

	//		if (ImGui::TreeNode(buffer))
	//		{
	//			add_to_hierarchy();
	//			ImGui::TreePop();
	//		}
	//	}

	//	
	//	
	//}

	//void add_transforms(Uint_32 id_transform)
	//{
	//	transform_queue.push(id_transform);
	//	Uint_32 parent_id = get_transform(id_transform).parent_transform;
	//	if (parent_id)
	//		add_transforms(parent_id);
	//}

	void add_child(Uint_32 id_transform)
	{
		char buffer[64];
		sprintf_s(buffer, "%d", id_transform);
		//transform_queue.pop();

		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGuiTreeNodeFlags node_flags = base_flags;
		if(selected_transform == id_transform)
			node_flags |= ImGuiTreeNodeFlags_Selected;
		
		bool node_open = ImGui::TreeNodeEx(buffer, node_flags, buffer, id_transform);
	
		
		if (ImGui::IsItemClicked())
			selected_transform = id_transform;



		if (node_open)
		{
			for (auto it : transform_children[id_transform])
			{
				if(entity_management_system->transform_entity_id[it])
					add_child(it);
			}
			ImGui::TreePop();
		}
	}

	void GuiLayer::draw_editor()
	{
		if (!ImGui::Begin("Scene Hierarchy", false))
		{
			ImGui::End();
			return;
		}
		
		Uint_32 max_hierarchy = 0;
		for (int _i = 0; _i< entity_management_system->next_transform_order; _i++)
		{
			auto id_t = transform_order[_i];
			if (entity_management_system->transform_entity_id[id_t])
			{
				if (transform_hierarchy_level[id_t] >= max_hierarchy)
				{
					max_hierarchy = transform_hierarchy_level[id_t];
					add_child(id_t);
				}
			}
		}
		ImGui::End();


		if (!ImGui::Begin("Inspector", false))
		{
			ImGui::End();
			return;
		}

		if (last_selected_transform != selected_transform) // selection changed, so remove outline
		{

			auto entity = entity_management_system->transform_entity_id[last_selected_transform];
			auto id_graphics = entities[entity][COMP_GRAPHICS];
			auto& graphic = graphics[id_graphics];
			graphic.outline_alpha = 0.0;
		}

		if (selected_transform)
		{
			Vector3<float>& pos = get_transform(selected_transform).position;
			float v3_p[3] = { pos.x, pos.y, pos.z };
			//ImGui::InputFloat("input float", &pos.x, 0.01f, 1.0f, "%.3f");
			ImGui::DragFloat3("Position", v3_p, 1.0f, -10000.0f, 10000.0f, "%.3f", 1.0f);
			pos.x = v3_p[0];
			pos.y = v3_p[1];
			pos.z = v3_p[2];

			Vector3<float>& rot = get_transform(selected_transform).rotation;
			float v3_r[3] = { rot.x, rot.y, rot.z };
			//ImGui::InputFloat("input float", &pos.x, 0.01f, 1.0f, "%.3f");
			ImGui::DragFloat3("Rotation", v3_r, 1.0f, -10000.0f, 10000.0f, "%.3f", 1.0f);
			rot.x = v3_r[0];
			rot.y = v3_r[1];
			rot.z = v3_r[2];

			Vector3<float>& scl = get_transform(selected_transform).scale;
			float v3_s[3] = { scl.x, scl.y, scl.z };
			//ImGui::InputFloat("input float", &pos.x, 0.01f, 1.0f, "%.3f");
			ImGui::DragFloat3("Scale", v3_s, 1.0f, -10000.0f, 10000.0f, "%.3f", 1.0f);
			scl.x = v3_s[0];
			scl.y = v3_s[1];
			scl.z = v3_s[2];

			auto entity = entity_management_system->transform_entity_id[selected_transform];
			auto id_graphics = entities[entity][COMP_GRAPHICS];
			auto& graphic = graphics[id_graphics];
			graphic.outline_alpha = 1.0;
			last_selected_transform = selected_transform;

			Vector2<Float_32> v1 = { 1.0f,1.0f};
			Vector3<Int_32> v2 = { 1, 1, 1};
			Vector2<Float_32> v3 = (Vector3<Float_32>)v2 + (Vector2<Float_32>)v1;

			
		}
		ImGui::End();

	}





}