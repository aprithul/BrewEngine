#include "GuiLayer.hpp"
#include "Logger.hpp"
#include "EntityManagementSystemModule.hpp"
#include "RendererOpenGL2D.hpp"
#include <queue>
#include "EntityGenerator.hpp"
#include "EditorUtils.hpp"
#include "PhysicsModule.hpp"
#include "Math.hpp"
#include "SceneManager.hpp"
#include "GizmoLayer.hpp"
namespace PrEngine
{

#ifdef EDITOR_MODE
	Uint_32 selected_transform = 0;
	Uint_32 last_selected_transform = 0;
	std::vector<std::string> material_directories;
	std::vector<std::string> shader_directories;
	std::vector<std::string> texture_directories;
	std::vector<std::string> animation_directories;
	std::vector<std::string> script_names;

	void load_materials();
	void load_textures();
	void load_animations();
	void load_shaders();

	Uint_32 is_mouse_in_any_graphic(Vec2f mouse_pos);
#endif // 



	Bool_8 is_mouse_inside_viewport(Vec2f mouse_pos)
	{
		Int_32 w, h;
		SDL_GetWindowSize(renderer->window, &w, &h);
		Rect<Float_32> r{ ImGui::min_viewport.x, (h - ImGui::min_viewport.y) - (ImGui::max_viewport.y - ImGui::min_viewport.y), ImGui::max_viewport.x - ImGui::min_viewport.x, ImGui::max_viewport.y - ImGui::min_viewport.y };
		return point_in_AABB(mouse_pos, r);
	}

    GuiLayer::GuiLayer(SDL_Window* sdl_window, SDL_GLContext* gl_context):window(sdl_window),gl_context(gl_context)
    {
        this->name = "GUI";
        panning = nullptr;
        tiling = nullptr;
		this->fps = 0;

		std::string file_content =  read_file(get_resource_path() + PATH_SEP + "type_names.csv");
		tokenize_string(file_content, ',', script_names);
	}

    GuiLayer::~GuiLayer()
    {
        
    }

    void GuiLayer::start()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGuiIO& io = ImGui::GetIO(); (void)io;
#ifdef _WIN64
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
#elif _SWITCH
		io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
#endif
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init("#version 300 es");
        //inspector initialization
        inspector_active = true;
        //ImGui::StyleColorsClassic();


#ifdef EDITOR_MODE

		load_materials();
		load_textures();
		load_animations();
		load_shaders();

		// gizmo arrows
		//EntityGenerator eg;
		//eg.make_sprite("Materials" + PATH_SEP + "Gizmo.mat", Point3d{ 2,0,0 }, RENDER_UNTAGGED);

#endif // EDITOR_MODE

    }

	void ShowExampleAppDockSpace(bool* p_open)
	{
		// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
		// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
		// In this specific demo, we are not using DockSpaceOverViewport() because:
		// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
		// - we allow the host window to have padding (when opt_padding == true)
		// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
		// TL;DR; this demo is more complicated than what you would normally use.
		// If we removed all the options we are showcasing, this demo would become:
		//     void ShowExampleAppDockSpace()
		//     {
		//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		//     }

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		//ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		
		/*
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
					*p_open = false;
				ImGui::EndMenu();
			}
			

			ImGui::EndMenuBar();
		}
		*/

		ImGui::End();
	}


    void GuiLayer::update()
    {

        IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!"); // Exceptionally add an extra assert here for people confused with initial dear imgui setup

		// imgui stuff
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = Time::Frame_time;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		static Bool_8 show = true;
		//ImGui::ShowDemoWindow();
		ShowExampleAppDockSpace(0);
#ifdef EDITOR_MODE
		draw_editor(window);
		//ImGui::ShowMetricsWindow();
		//ImGui::ShowDemoWindow();
		

		if (input_manager->keyboard.get_key_down(SDLK_F5))
		{
			SceneManager::get_instance()->save_current_scene();
			LOG(LOGTYPE_WARNING, "scene file saved");
		}

		if (input_manager->keyboard.get_key_down(SDLK_ESCAPE))
		{
			input_manager->was_crossed = true;
		}

	

#endif // EDITOR_MODE

#ifdef DEBUG

		draw_debug_window(io.Framerate);
#endif // DEBUG

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GuiLayer::end()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
    }
	
#ifdef EDITOR_MODE

	void add_child(Uint_32 id_transform)
	{
		char buffer[64];
#ifdef _WIN64
		sprintf_s(buffer, "%d", id_transform);
#elif	_SWITCH
		sprintf(buffer, "%d", id_transform);
#endif // _WIN64

		//transform_queue.pop();

		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGuiTreeNodeFlags node_flags = base_flags;
		if(selected_transform == id_transform)
			node_flags |= ImGuiTreeNodeFlags_Selected;
		
		bool node_open = ImGui::TreeNodeEx(buffer, node_flags, buffer, id_transform);
	
		
		if (ImGui::IsItemClicked())
		{
			selected_transform = id_transform;
			GizmoLayer::move_gizmo.target_transform = selected_transform;
		}

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
			for (auto it : *transforms[id_transform].children)
			{
				if(transform_entity_id[it])
					add_child(it);
			}
			ImGui::TreePop();
		}
	}

	Float_32 drag_amount;
	Float_32 drag_limit = 20;
	Vec2f drag_start;
	Vec2f mouse_pos_ws;
	//Vec2f mouse_pos_vs;
	Vec2f mouse_pos_ss;
	Uint_32 drag_transform;
	float v_x, v_y, v_w, v_h;
	void GuiLayer::draw_editor(SDL_Window* window)
	{
		//axis lines
		renderer->draw_line(Vec3f{0, 0, 0}, Vec3f{100, 0, 0}, Vec4f{1, 0, 0, 1});
		renderer->draw_line(Vec3f{0, 0, 0}, Vec3f{0, 100, 0}, Vec4f{0, 1, 0, 1});




		// editor input section
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Uint_32 cam = entity_management_system->get_active_camera();
		Uint_32 cam_transform = cameras[cam].id_transform;
		//mouse_pos_ws = cameras[cam].get_screen_to_world_pos(input_manager->mouse.position);
		//mouse_pos_vs = Vec2f{ clamp<Float_32>(input_manager->mouse.position.x - v_x, 0, v_w) ,
		//	clamp<Float_32>(renderer->height - (input_manager->mouse.position.y - v_y), 0, v_h) };

		// narrowing shouldn't cause problem in screen space (limited space)
		mouse_pos_ss = cameras[cam].get_mouse_to_screen_pos(input_manager->mouse.position);
		mouse_pos_ws = (Vec2f)(cameras[cam].get_screen_to_world_pos(mouse_pos_ss));

		// entity selection by clicking sprite
		static Vec2f selection_offset;
		if (input_manager->mouse.get_mouse_button_down(1))
		{
			Uint_32 clicked_on_graphic= is_mouse_in_any_graphic(mouse_pos_ws);
			if (clicked_on_graphic)
			{
				Uint_32 clicked_on_transform = graphics[clicked_on_graphic].transform_id;
				if (selected_transform == clicked_on_transform)	// 
				{
					drag_transform = selected_transform;
					selection_offset = (Vec2f)transforms[selected_transform].get_global_position() - mouse_pos_ws;
				}
				else
				{
					selected_transform = clicked_on_transform;
					drag_transform = 0;
				}
			}
			else
			{
				// check if click was inside viewport
				if(is_mouse_inside_viewport(mouse_pos_ss))
				{
					selected_transform = 0;
					GizmoLayer::move_gizmo.target_transform = 0;
					drag_transform = 0;
				}
			}
		}
		// draw rect around selected entity
		if (selected_transform)
		{
			const Mat4x4& _transform = transforms[selected_transform].transformation;
			Uint_32 entity_id = transform_entity_id[selected_transform];
			Vec3f* vertices = Graphic::vertex_data[entities[entity_id][COMP_GRAPHICS]];

			Rect<Float_32> rect = points_to_rect(vertices);
			Vec4f color{ 0.8, 0.5, 0, 1 };
			renderer->draw_rect_with_transform(rect, color,_transform);

		}
		//drag grpahic with mouse pointer
		if (drag_transform)
		{
			if (input_manager->mouse.get_mouse_button(1))
			{
				Point3d drag_transform_pos = transforms[drag_transform].get_global_position();
				drag_transform_pos.x = mouse_pos_ws.x + selection_offset.x;
				drag_transform_pos.y = mouse_pos_ws.y + selection_offset.y;
				transforms[drag_transform].set_global_position(drag_transform_pos);
			}
			else
				drag_transform = 0;
		}
		
		
		Float_32 cam_pan_min_speed = 4.f;
		Float_32 cam_pan_max_speed = 10.f;
		Float_32 cam_pan_speed = cam_pan_min_speed;
		if (input_manager->keyboard.get_key(SDLK_LSHIFT))
			cam_pan_speed = cam_pan_max_speed;

		Point3d pos = transforms[cam_transform].get_local_position();
		if (input_manager->keyboard.get_key(SDLK_w))
			pos.y = pos.y + (Time::Frame_time*cam_pan_speed);
		if (input_manager->keyboard.get_key(SDLK_s))
			pos.y = pos.y - (Time::Frame_time*cam_pan_speed);
		if (input_manager->keyboard.get_key(SDLK_d))
			pos.x = pos.x + (Time::Frame_time*cam_pan_speed);
		if (input_manager->keyboard.get_key(SDLK_a))
			pos.x = pos.x - (Time::Frame_time*cam_pan_speed);
		transforms[cam_transform].set_local_position(pos);

		if (input_manager->mouse.scroll != 0 && is_mouse_inside_viewport(mouse_pos_ss))
		{
			Float_32 speed = 20.f;
			cameras[cam].zoom = clamp<Float_32>(cameras[cam].zoom + +(input_manager->mouse.scroll*Time::Frame_time*speed), 0.1, 1000);
		}

		if (input_manager->keyboard.get_key_down(SDLK_DELETE))
			entity_management_system->delete_entity_by_transform(selected_transform);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		draw_scene_hierarchy();
		draw_inspector_window();
		draw_asset_window();

		Uint_32 cam_id = entity_management_system->get_active_camera();
		if (cam_id)
		{
			Int_32 w, h;
			SDL_GetWindowSize(window, &w, &h);

			Camera& cam = cameras[cam_id];
			Float_32 cam_asp_ratio = cam.width / cam.height;

			Int_32 center_hole_w = ImGui::max_viewport.x - ImGui::min_viewport.x;
			Int_32 center_hole_h = ImGui::max_viewport.y - ImGui::min_viewport.y;
			if (center_hole_w == 0 || center_hole_h == 0)
				return;
			

			Float_32 hole_asp_ratio = center_hole_w / (Float_32)center_hole_h;
			static int c_ = 100;
			c_--;

			if (hole_asp_ratio >= cam_asp_ratio)
			{
				Int_32 center_hole_half_y = (h - (ImGui::max_viewport.y + ImGui::min_viewport.y) / 2);
				Int_32 target_h = center_hole_w / (Float_32)cam_asp_ratio;
				Int_32 target_min_y = center_hole_half_y - (target_h / 2);
				//Int_32 target_max_y = center_hole_half_y + (target_h / 2);

				renderer->update_viewport_size(ImGui::min_viewport.x, target_min_y, center_hole_w, target_h);

			}
			else
			{
				Int_32 center_hole_half_x = (ImGui::max_viewport.x + ImGui::min_viewport.x) / 2;
				Int_32 target_w = center_hole_h * cam_asp_ratio;
				Int_32 target_min_x = center_hole_half_x - (target_w / 2);
				//Int_32 target_max_x = center_hole_half_x + (target_w / 2);

				renderer->update_viewport_size(target_min_x, h - ImGui::max_viewport.y, target_w, center_hole_h);

				//renderer->update_viewport_size(ImGui::min_viewport.x, h - ImGui::max_viewport.y,
				//	ImGui::max_viewport.x - ImGui::min_viewport.x, ImGui::max_viewport.y - ImGui::min_viewport.y);
			}

			//cam.v_mod = (cam_w / (v_ar*cam_h));
			//renderer->update_viewport_size(v_x, v_y, v_w, v_h);

		}
	}

	void GuiLayer::draw_inspector_window()
	{
		// flags and data for menubar
		static Bool_8 call_add_script = false;
		static std::string script_to_add = ""; 

		// menubar actions
		if (call_add_script)
		{
			//add_script(script_to_call);
			Script* script = Scripting::get_script_instance(script_to_add.c_str());
			Uint_32 sel_tr_entity = transform_entity_id[selected_transform];
			entity_management_system->add_script_to_entity(sel_tr_entity, script, script_to_add.c_str());
			call_add_script = false;
		}



		ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
		if (!ImGui::Begin("Inspector",0, flags))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginMenuBar())
		{
			if (selected_transform)
			{
				if (ImGui::BeginMenu("Add..."))
				{
					ImGui::MenuItem("Graphics", NULL);
					ImGui::MenuItem("Animator", NULL);
					ImGui::MenuItem("Collider", NULL);
					if (ImGui::BeginMenu("Script"))
					{
						for (auto script_name : script_names)
						{
							ImGui::MenuItem(script_name.c_str(),0, &call_add_script);
							script_to_add = script_name;
						}
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
			}
			ImGui::EndMenuBar();
		}

		//	ImGui::EndMenu();
		//}		//v_w = ImGui::GetWindowPos().x - v_x;


		if (selected_transform)
		{
			Uint_32 entity = transform_entity_id[selected_transform];
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				Point3d pos = transforms[selected_transform].get_local_position();// .get_global_position();
				float v3_p[3] = { pos.x, pos.y, pos.z };
				//ImGui::InputFloat("input float", &pos.x, 0.01f, 1.0f, "%.3f");
				ImGui::DragFloat3("Position", v3_p, 1.0f, -10000.0f, 10000.0f, "%.3f", 1.0f);
				pos.x = v3_p[0];
				pos.y = v3_p[1];
				pos.z = v3_p[2];
				transforms[selected_transform].set_local_position(pos);
				//transforms[selected_transform].position = transforms[selected_transform].get_global_to_local_position(pos);

				Vec3f rot = Quaternion::QuaternionToEuler(transforms[selected_transform].get_local_rotation());// transforms[selected_transform].get_local_rotation();// get_global_rotation();
				float v3_r[3] = { rot.x, rot.y, rot.z };
				//ImGui::InputFloat("input float", &pos.x, 0.01f, 1.0f, "%.3f");
				ImGui::DragFloat3("Rotation", v3_r, 1.0f, -10000.0f, 10000.0f, "%.3f", 1.0f);
				rot.x = v3_r[0];
				rot.y = v3_r[1];
				rot.z = v3_r[2];
				transforms[selected_transform].set_local_rotation(rot);
				//transform_editor_data[selected_transform] = rot;
				//transforms[selected_transform].rotation = transforms[selected_transform].get_global_to_local_rotation(rot);

				Vec3f scl = transforms[selected_transform].get_local_scale();
				float v3_s[3] = { scl.x, scl.y, scl.z };
				//ImGui::InputFloat("input float", &pos.x, 0.01f, 1.0f, "%.3f");
				ImGui::DragFloat3("Scale", v3_s, 1.0f, -10000.0f, 10000.0f, "%.3f", 1.0f);
				scl.x = v3_s[0];
				scl.y = v3_s[1];
				scl.z = v3_s[2];
				transforms[selected_transform].set_local_scale(scl);

			}
			last_selected_transform = selected_transform;

			auto& ent = entities[entity];
			if (ent[COMP_GRAPHICS])
			{
				if (ImGui::CollapsingHeader("Graphics", ImGuiTreeNodeFlags_DefaultOpen))
				{
					Uint_32 graphic_id = ent[COMP_GRAPHICS];
					Graphic& g = graphics[graphic_id];
					//static Int_32 _tag = (Int_32)g.tag;
					ImGui::Combo("Render Mode", &Graphic::editor_data[graphic_id].future_tag, "Untagged\0Static\0Dynamic\0", 3);
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
					ImGui::Checkbox("Translate", &animator.anim_transform_update_flags[ANIM_TRANSLATE]);
					ImGui::Checkbox("Rotate", &animator.anim_transform_update_flags[ANIM_ROTATE]);
					ImGui::Checkbox("Scale", &animator.anim_transform_update_flags[ANIM_SCALE]);

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

	void GuiLayer::draw_scene_hierarchy()
	{
		//if (!ImGui::Begin("Scene Hierarchy", 0, ImGuiWindowFlags_NoMove))
		if (!ImGui::Begin("Scene Hierarchy", 0))
		{
			ImGui::End();
			return;
		}

		//v_x = ImGui::GetWindowPos().x + ImGui::GetWindowWidth();

		//Uint_32 max_hierarchy = 0;
		for (int _i = 0; _i < next_transform_pos; _i++)
		{
			if (transform_entity_id[_i])
			{
				if (transforms[_i].parent_transform == 0)// max_hierarchy)
				{
					//max_hierarchy = transform_hierarchy_level[id_t];
					add_child(_i);

				}
			}
		}

		const ImGuiPayload* payload = ImGui::GetDragDropPayload();
		if (input_manager->mouse.get_mouse_button_up(1) && payload)
		{
			if (payload && !payload->Delivery)
			{
				if (payload->DataSize == sizeof(Uint_32))
				{
					Uint_32 dragged_transform_id = *(const int*)payload->Data;
					entity_management_system->set_parent_transform(0, dragged_transform_id);
				}
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

	void GuiLayer::draw_asset_window()
	{
		if (!ImGui::Begin("Assets", 0))//, ImGuiWindowFlags_NoMove))
		{
			ImGui::End();
			return;
		}

		/*v_y = renderer->height - ImGui::GetWindowPos().y;// -ImGui::GetWindowHeight());// ImGui::GetWindowPos().y;// (renderer->height - (ImGui::GetWindowPos().y - ImGui::GetWindowHeight()));
		v_h = renderer->height - ImGui::GetWindowHeight();
		
		ImVec2 _pos = ImGui::GetWindowPos();
		ImVec2 _size = ImGui::GetWindowSize();
		_pos.x = v_x;
		//ImGui::SetWindowPos(_pos);
		_size.x = v_w;
		//ImGui::SetWindowSize(_size);*/
		
		static int selected = 0;
		const char* labels[4] = { "Material", "Shaders", "Textures", "Animations" };

		ImGui::Columns(2, "Asset columns", 0);  // 2-ways, no border

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

	Uint_32 is_mouse_in_any_graphic(Vec2f mouse_pos)
	{
		for (auto& it : Graphic::vertex_data)
		{
			Uint_32 t_id = graphics[it.first].transform_id;
			Vec2f points[4];
			points[0] = transforms[t_id].transformation * it.second[0];
			points[1] = transforms[t_id].transformation * it.second[1];
			points[2] = transforms[t_id].transformation * it.second[2];
			points[3] = transforms[t_id].transformation * it.second[3];

			if (point_in_shape(points, 4, mouse_pos))
				return it.first;
		}

		return 0;
	}


	void GuiLayer::add_script(Uint_32 entity, std::string& script_name)
	{
	}
#endif

#ifdef DEBUG
	void GuiLayer::draw_debug_window(Float_32 fps)
	{
		if (!ImGui::Begin("Info: ", 0))//, ImGuiWindowFlags_NoMove))
		{
			ImGui::End();
			return;
		}
#ifdef EDITOR_MODE

		/*ImVec2 _pos = ImGui::GetWindowPos();
		ImVec2 _size = ImGui::GetWindowSize();
		_pos.x = v_x + v_w - _size.x;
		ImGui::SetWindowPos(_pos);*/

		ImGui::Text(("(WS): " + std::to_string(mouse_pos_ws.x)+","+ std::to_string(mouse_pos_ws.y)).c_str());
		ImGui::Text(("(SS): " + std::to_string(mouse_pos_ss.x)+","+ std::to_string(mouse_pos_ss.y)).c_str());
#endif
		ImGui::Text("(%.1f FPS)", fps);
		
		ImGui::End();
	}
#endif // DEBUG

}