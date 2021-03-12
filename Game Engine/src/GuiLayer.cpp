#include "GuiLayer.hpp"
#include "Logger.hpp"
#include "EntityManagementSystemModule.hpp"
#include "RendererOpenGL2D.hpp"
#include <queue>
#include "EntityGenerator.hpp"
#include "EditorUtils.hpp"

namespace PrEngine
{
	Uint_32 selected_transform = 0;
	Uint_32 last_selected_transform = 0;
	std::vector<std::string> material_directories;
	std::vector<std::string> shader_directories;
	std::vector<std::string> texture_directories;
	std::vector<std::string> animation_directories;

	void load_materials();
	void load_shaders();
	void load_textures();
	void load_animations();
	void draw_scene_hierarchy();
	void draw_inspector_window();
	void draw_asset_window();

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
		
		load_materials();
		load_textures();
		load_animations();
		load_shaders();
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
		Vector2<Float_32> mouse_pos_ws = cameras[cam].get_screen_to_world_pos(input_manager->mouse.position);
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


		/*if (input_manager->keyboard.get_key_down(SDLK_l))
		{
			static float _x = -5;
			static float _y = -5;
			static float _z = -5;
			_x += 5 * (Float_32)Time::Frame_time;
			_y += 5 * (Float_32)Time::Frame_time;
			_z += 5 * (Float_32)Time::Frame_time;
			renderer->draw_line(Vector2<Float_32>{0, 0}, Vector2<Float_32>{_x, -2.f}, Vector4<Float_32>{1, 0, 0, 1});
			renderer->draw_line(Vector2<Float_32>{0, 0}, Vector2<Float_32>{100, 0}, Vector4<Float_32>{0, 1, 0, 1});
			renderer->draw_line(Vector2<Float_32>{0, 0}, Vector2<Float_32>{-2.f, -_x}, Vector4<Float_32>{0, 0, 1, 1});
		}*/

		if (input_manager->keyboard.get_key_down(SDLK_c))
		{
			std::string material_name = "Materials\\Door.mat";
			EntityGenerator eg;
			auto e = eg.make_graphics_entity(material_name);
			auto t_id = entities[e][COMP_TRANSFORM_3D];
			auto& pos = transforms[t_id].position;
			pos.x = rand() % 4* (rand() % 2 ? 1 : -1);
			pos.y = rand() % 2* (rand() % 2 ? 1 : -1);

			mouse_pointer_transform = t_id;
		}

		if (mouse_pointer_transform)
		{
			transforms[mouse_pointer_transform].position = mouse_pos_ws;
		}

		draw_editor();
		ImGui::ShowMetricsWindow();
		ImGui::ShowDemoWindow();
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

		//if (input_manager->mouse.get_mouse_button(SDL_BUTTON_LEFT) && selected_transform)
		//{
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("SCENE_HIERARCHY", &selected_transform, sizeof(Uint_32));
				ImGui::Text("Copy %d", selected_transform);
				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY");
				if (payload)
				{
					IM_ASSERT(payload->DataSize == sizeof(Uint_32));
					Uint_32 dragged_transform_id = *(const int*)payload->Data;
					entity_management_system->set_parent_transform(id_transform, dragged_transform_id);
				}
				ImGui::EndDragDropTarget();
			}

		//}


		if (node_open)
		{
			for (auto it : transform_children[id_transform])
			{
				if(transform_entity_id[it])
					add_child(it);
			}
			ImGui::TreePop();
		}
	}

	Float_32 drag_amount;
	Float_32 drag_limit = 20;
	Vector2<Int_32> drag_start;
	void GuiLayer::draw_editor()
	{
		draw_scene_hierarchy();
		draw_inspector_window();
		draw_asset_window();
	}

	void draw_inspector_window()
	{
		if (!ImGui::Begin("Inspector", false))
		{
			ImGui::End();
			return;
		}

		/*Transform*/
		if (last_selected_transform != selected_transform) // selection changed, so remove outline
		{
			auto entity = transform_entity_id[last_selected_transform];
			auto id_graphics = entities[entity][COMP_GRAPHICS];
			auto& graphic = graphics[id_graphics];
			graphic.outline_alpha = 0.0;
		}

		if (selected_transform)
		{
			Uint_32 entity = transform_entity_id[selected_transform];
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				Vector3<float>& pos = transforms[selected_transform].position;// .get_global_position();
				float v3_p[3] = { pos.x, pos.y, pos.z };
				//ImGui::InputFloat("input float", &pos.x, 0.01f, 1.0f, "%.3f");
				ImGui::DragFloat3("Position", v3_p, 1.0f, -10000.0f, 10000.0f, "%.3f", 1.0f);
				pos.x = v3_p[0];
				pos.y = v3_p[1];
				pos.z = v3_p[2];
				//transforms[selected_transform].position = transforms[selected_transform].get_global_to_local_position(pos);

				Vector3<float>& rot = get_transform(selected_transform).rotation;// get_global_rotation();
				float v3_r[3] = { rot.x, rot.y, rot.z };
				//ImGui::InputFloat("input float", &pos.x, 0.01f, 1.0f, "%.3f");
				ImGui::DragFloat3("Rotation", v3_r, 1.0f, -10000.0f, 10000.0f, "%.3f", 1.0f);
				rot.x = v3_r[0];
				rot.y = v3_r[1];
				rot.z = v3_r[2];
				//transforms[selected_transform].rotation = transforms[selected_transform].get_global_to_local_rotation(rot);

				Vector3<float>& scl = get_transform(selected_transform).scale;
				float v3_s[3] = { scl.x, scl.y, scl.z };
				//ImGui::InputFloat("input float", &pos.x, 0.01f, 1.0f, "%.3f");
				ImGui::DragFloat3("Scale", v3_s, 1.0f, -10000.0f, 10000.0f, "%.3f", 1.0f);
				scl.x = v3_s[0];
				scl.y = v3_s[1];
				scl.z = v3_s[2];
			}
			auto id_graphics = entities[entity][COMP_GRAPHICS];
			auto& graphic = graphics[id_graphics];
			auto& mat = Material::material_library[graphic.element.material];
			graphic.outline_alpha = 1.0;
			last_selected_transform = selected_transform;

			auto& ent = entities[entity];
			if (ent[COMP_GRAPHICS])
			{
				if (ImGui::CollapsingHeader("Graphics", ImGuiTreeNodeFlags_DefaultOpen))
				{
					Graphic& g = graphics[ent[COMP_GRAPHICS]];
					//static Int_32 _tag = (Int_32)g.tag;
					ImGui::Combo("Render Mode", &g.future_tag, "Untagged\0Static\0Dynamic\0", 3);
					//g.tag = (RenderTag)_tag;

					ImGui::SetNextItemOpen(true);
					if (ImGui::TreeNode("Material"))
					{
						Uint_32 mat_id = g.element.material;
						if (mat_id)
						{
							std::string& mat_name = Material::material_names[mat_id];
							ImGui::Text(mat_name.c_str());
							if (ImGui::BeginDragDropTarget())
							{
								const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Material");
								if (payload)
								{
									IM_ASSERT(payload->DataSize == sizeof(Int_32));
									auto p = material_directories[*(const Int_32*)payload->Data];
									Uint_32 mat_id = Material::load_material(p, true);
									if (mat_id)
										g.element.material = mat_id;// *(const int*)payload->Data;
									else
										LOG(LOGTYPE_ERROR, "material couldn't be created");
								}
								ImGui::EndDragDropTarget();

							}
						}
						ImGui::TreePop();
					}
				}
			}

			if (ent[COMP_LIGHT])
			{
				if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
				{

				}
			}

			if (ent[COMP_CAMERA])
			{
				if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
				{

				}
			}

			if (ent[COMP_ANIMATOR])
			{
				if (ImGui::CollapsingHeader("Animator", ImGuiTreeNodeFlags_DefaultOpen))
				{

					Uint_32 id_animator = ent[COMP_ANIMATOR];
					Animator& animator = animators[id_animator];
					ImGui::DragFloat("Animation Speed", &animator.animation_speed, 0.05f, 0.0f, 5.0f);
					ImGui::InputInt("Current Animation", &animator.cur_anim_ind);
					if (ImGui::BeginDragDropTarget())
					{
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Animation");
						if (payload)
						{
							IM_ASSERT(payload->DataSize == sizeof(Int_32));
							Uint_32 anim_index = *(const Int_32*)payload->Data;
							//Uint_32 anim_id = Animator::load_animation(animation_directories[*(const Int_32*)payload->Data]);
								//animator.add_animation(anim_id);
							animator.cur_anim_ind = anim_index;// *(const int*)payload->Data;
						}
						ImGui::EndDragDropTarget();
					}
					ImGui::Checkbox("Translate", &animator.anim_flags[ANIM_TRANSLATE]);
					ImGui::Checkbox("Rotate", &animator.anim_flags[ANIM_ROTATE]);
					ImGui::Checkbox("Scale", &animator.anim_flags[ANIM_SCALE]);

					ImGui::BeginChild("Animations",ImVec2(0,0),true,0);

					//Int_32 no_of_animations = 0;
					for (int _i = 0; _i < animator.animation_count; _i++)
					{
							//no_of_animations++;
							Animation& animation = Animator::animations_library[animator.animation_ids[_i]];
							std::string _node_label = std::to_string(_i) + "_" + Animator::animation_clip_names[animator.animation_ids[_i]];
							if (ImGui::TreeNode(_node_label.c_str()))
							{
								ImGui::TreePop();
							}
							if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
							{
								/*Uint_32 anim = 0;
								for (int k = 0; k < animation_directories.size(); k++)
								{
									if (animation_directories[k] == animation.clip_name)
										anim = k;
								}*/

								ImGui::SetDragDropPayload("Animation", &_i, sizeof(Int_32));
								ImGui::Text(_node_label.c_str());
								ImGui::EndDragDropSource();
							}

					}
					ImGui::EndChild();

					if (ImGui::BeginDragDropTarget())
					{
						
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Animation Asset");
						if (payload)
						{
							Uint_32 anim_i = Animator::load_animation(animation_directories[*(const Int_32*)payload->Data]);
							if (anim_i)
							{
								animator.add_animation(anim_i);
							}
							else
							{
								LOG(LOGTYPE_ERROR, "Animation couldn't be loaded");
							}

						}

					}

					animator.cur_anim_ind = clamp<Int_32>(animator.cur_anim_ind, 0, animator.animation_count-1);
				}
			}
		}

		ImGui::End();
	}

	void draw_scene_hierarchy()
	{
		if (!ImGui::Begin("Scene Hierarchy", false))
		{
			ImGui::End();
			return;
		}



		Uint_32 max_hierarchy = 0;
		for (int _i = 0; _i < entity_management_system->next_transform_order; _i++)
		{
			auto id_t = transform_order[_i];
			if (transform_entity_id[id_t])
			{
				if (transform_hierarchy_level[id_t] == MAX_HIERARCHY_LEVEL)// max_hierarchy)
				{
					max_hierarchy = transform_hierarchy_level[id_t];
					add_child(id_t);

				}
			}
		}

		const ImGuiPayload* payload = ImGui::GetDragDropPayload();
		if (input_manager->mouse.get_mouse_button_up(1) && payload)
		{
			if (payload && !payload->Delivery)
			{
				IM_ASSERT(payload->DataSize == sizeof(Uint_32));
				Uint_32 dragged_transform_id = *(const int*)payload->Data;
				entity_management_system->set_parent_transform(0, dragged_transform_id);
				//transforms[dragged_transform_id].parent_transform = 0;
			}
			//ImGui::EndDragDropTarget();
		}

		ImGui::End();

	}

	void load_materials()
	{
		std::string dir = get_resource_path("");// +"Materials" + PATH_SEP;
		get_files_in_dir(dir, ".mat", material_directories);
		for (int i=0; i<material_directories.size(); i++)
		{
			auto p = material_directories[i].substr(dir.size());
			material_directories[i] = p;
			//Material::load_material(mat_path, false);
		}
	}

	void load_shaders()
	{
		std::string dir = get_resource_path("");// +"Materials" + PATH_SEP;
		get_files_in_dir(dir, ".shader", shader_directories);
		for (int i = 0; i < shader_directories.size(); i++)
		{
			auto p = shader_directories[i].substr(dir.size());
			shader_directories[i] = p;
			//Material::load_material(mat_path, false);
		}
	}

	void load_textures()
	{
		std::string dir = get_resource_path("");// +"Materials" + PATH_SEP;
		get_files_in_dir(dir, ".png", texture_directories);
		get_files_in_dir(dir, ".jpg", texture_directories);
		for (int i = 0; i < texture_directories.size(); i++)
		{
			auto p = texture_directories[i].substr(dir.size());
			texture_directories[i] = p;
			//Material::load_material(mat_path, false);
		}
	}

	void load_animations()
	{
		std::string dir = get_resource_path("");// +"Materials" + PATH_SEP;
		get_files_in_dir(dir, ".anim", animation_directories);
		for (int i = 0; i < animation_directories.size(); i++)
		{
			auto p = animation_directories[i].substr(dir.size());
			animation_directories[i] = p;
			//Material::load_material(mat_path, false);
		}
	}

	void draw_asset_window()
	{
		if (!ImGui::Begin("Assets", false))
		{
			ImGui::End();
			return;
		}
		static int selected = 0;
		const char* labels[4] = { "Material", "Shaders", "Textures", "Animations" };

		ImGui::Columns(2, "Asset columns", false);  // 2-ways, no border

		for (int i = 0; i < 4; i++)
		{
			if (ImGui::Selectable(labels[i], selected == i))
			{
				selected = i;
			}
		}

		ImGui::NextColumn();
		ImGui::BeginChild("Scrollable panel");
		static int selected_col_2 = 0;

		if (selected == 0)
		{
			for (int j = 0; j < material_directories.size(); j++)
			{
				if (ImGui::Selectable(material_directories[j].c_str(), selected_col_2 == j))
				{
					selected_col_2 = j;
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("Material", &j, sizeof(Int_32));
					ImGui::Text(material_directories[j].c_str());
					ImGui::EndDragDropSource();
				}
			}
		}
		else if (selected == 1)
		{
			for (int j = 0; j < shader_directories.size(); j++)
			{
				if (ImGui::Selectable(shader_directories[j].c_str(), selected_col_2 == j))
				{
					selected_col_2 = j;
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("Shader", &j, sizeof(Int_32));
					ImGui::Text(shader_directories[j].c_str());
					ImGui::EndDragDropSource();
				}
			}
		}
		else if (selected == 2)
		{
			for (int j = 0; j < texture_directories.size(); j++)
			{
				if (ImGui::Selectable(texture_directories[j].c_str(), selected_col_2 == j))
				{
					selected_col_2 = j;
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("Texture", &j, sizeof(Int_32));
					ImGui::Text(texture_directories[j].c_str());
					ImGui::EndDragDropSource();
				}
			}
		}
		else if (selected == 3)
		{
			for (int j = 0; j < animation_directories.size(); j++)
			{
				if (ImGui::Selectable(animation_directories[j].c_str(), selected_col_2 == j))
				{
					selected_col_2 = j;
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("Animation Asset", &j, sizeof(Int_32));
					ImGui::Text(animation_directories[j].c_str());
					ImGui::EndDragDropSource();
				}
			}
		}

		ImGui::EndChild();
		ImGui::End();
	}
}