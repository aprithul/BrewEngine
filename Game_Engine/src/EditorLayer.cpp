#include "EditorLayer.hpp"
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
#include "ImGuiConfiguration.hpp"

namespace PrEngine
{

	Uint_32 selected_transform = 0;
	Uint_32 last_selected_transform = 0;
	std::vector<std::string> material_directories;
	std::vector<std::string> shader_directories;
	std::vector<std::string> texture_directories;
	std::vector<std::string> animation_directories;
	static Uint_32 rename_transform = 0;
	Bool_8 is_editor_on = 1;

	Bool_8 do_edit_collider = false;
	Uint_32 edit_collider_id = 0;
	Bool_8 inside_collider_edit_area = false;
	Bool_8 edge_id[4] = {};	// top, left, bottom, right
	void load_materials();
	void load_textures();
	void load_animations();
	void load_shaders();

	Uint_32 is_mouse_in_any_graphic(Vec2f mouse_pos);



	Bool_8 is_mouse_inside_viewport(Vec2f mouse_pos)
	{
		Int_32 w, h;
		SDL_GetWindowSize(renderer->window, &w, &h);
		Rect<Float_32> r{ ImGui::min_viewport.x, (h - ImGui::min_viewport.y) - (ImGui::max_viewport.y - ImGui::min_viewport.y), ImGui::max_viewport.x - ImGui::min_viewport.x, ImGui::max_viewport.y - ImGui::min_viewport.y };
		return point_in_AABB(mouse_pos, r);
	}

    EditorLayer::EditorLayer(SDL_Window* sdl_window, SDL_GLContext* gl_context):window(sdl_window),gl_context(gl_context)
    {
        this->name = "GUI";
        panning = nullptr;
        tiling = nullptr;
		this->fps = 0;
		
		{
			PerformanceTimer pt;
			load_textures();
			load_materials();
			load_animations();
			load_shaders();
		}
		char c = 821;
		unsigned char uc = 821;
		char nc = -129;
		unsigned char nuc = -129;
		std::cout << "hello";

	}

	EditorLayer::~EditorLayer()
    {
        
    }

    void EditorLayer::start()
    {

		
		// gizmo arrows
		//EntityGenerator eg;
		//eg.make_sprite("Materials" + PATH_SEP + "Gizmo.mat", Point3d{ 2,0,0 }, RENDER_UNTAGGED);


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

	void setup_viewport(SDL_Window* window)
	{
		Uint_32 cam_id = entity_management_system->get_active_camera();
		if (cam_id)
		{
			Int_32 w, h;
			SDL_GetWindowSize(window, &w, &h);

			Camera& camera = camera_system.get_component(cam_id);
			Float_32 cam_asp_ratio = camera.width / camera.height;

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


    void EditorLayer::update()
    {

        IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!"); // Exceptionally add an extra assert here for people confused with initial dear imgui setup

		// imgui stuff
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = Time::Frame_time;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		static Bool_8 show = true;
		ShowExampleAppDockSpace(0);
		if(is_editor_on)
			draw_editor(window);
		//ImGui::ShowMetricsWindow();
		//ImGui::ShowDemoWindow();
		setup_viewport(window);

		if (input_manager->keyboard.get_key_down(SDLK_F5))
		{
			SceneManager::get_instance()->save_current_scene();
			LOG(LOGTYPE_WARNING, "scene file saved");
		}

		if (input_manager->keyboard.get_key_down(SDLK_F10))
		{
			is_editor_on = !is_editor_on;
		}

		if (input_manager->keyboard.get_key_down(SDLK_ESCAPE))
		{
			input_manager->was_crossed = true;
		}

		if (input_manager->keyboard.get_key(SDLK_LCTRL) && input_manager->keyboard.get_key_down(SDLK_d))
		{
			LOG(LOGTYPE_GENERAL, "Duplicate ", std::to_string(selected_transform));

			EntityGenerator eg;
			Uint_32 selected_entity = transform_system.get_entity(selected_transform);
			eg.duplicate_entity(selected_entity);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void EditorLayer::end()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
    }
	

	void add_child(Uint_32 id_transform)
	{
		Uint_32 entity = transform_system.get_entity(id_transform);
		char entity_name[128];
		std::memcpy(entity_name, EntityManagementSystem::entity_names[entity].c_str(), 128);
//		char buffer[64];
//#ifdef _WIN64
//		sprintf_s(buffer, "%s", EntityManagementSystem::entity_names[entity].c_str());
//#elif	_SWITCH
//		sprintf(buffer, "%d", id_transform);
//#endif // _WIN64
//
		//transform_queue.pop();

		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGuiTreeNodeFlags node_flags = base_flags;
		if(selected_transform == id_transform)
			node_flags |= ImGuiTreeNodeFlags_Selected;
		
		if (rename_transform == id_transform)
		{

		}
		bool node_open = false;
		if (rename_transform == id_transform)
		{
			//ImGui::SetItemAllowOverlap();
			ImVec2 prev_cursor_pos = ImGui::GetCursorPos();
			//ImGui::SetCursorPos(node_cursor_pos);
			//ImGui::GetTextLineHeight();
			char buffer[128];
			Uint_32 selected_entity = transform_system.get_entity(rename_transform);
			sprintf(buffer, "%s", EntityManagementSystem::entity_names[selected_entity].c_str());

			ImGui::PushID(&rename_transform);

			ImGui::SetKeyboardFocusHere();
			ImGui::InputText("", buffer, 128, ImGuiInputTextFlags_AutoSelectAll);
			ImGui::PopID();

			EntityManagementSystem::entity_names[selected_entity] = std::string(buffer);
		}
		else
		{
			ImGui::PushID(id_transform);
			node_open = ImGui::TreeNodeEx(entity_name, node_flags, entity_name, id_transform);
			ImGui::PopID();
		}

		
		if (ImGui::IsItemClicked())
		{
			if (selected_transform != id_transform)
			{
				do_edit_collider = false;
				edit_collider_id = 0;
				//collider_being_edited = 0;
			}
			
			selected_transform = id_transform;
			GizmoLayer::move_gizmo.target_transform = selected_transform;
		}



		if (input_manager->keyboard.get_key_down(SDLK_F2))
		{
			rename_transform = selected_transform;
		}

		if (selected_transform != rename_transform || input_manager->keyboard.get_key_down(SDLK_RETURN))
		{
			rename_transform = 0;

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
			Transform3D& _transform = transform_system.get_component(id_transform);
			for (auto it : *_transform.children)
			{
				if(transform_system.get_entity(it))
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

	void EditorLayer::draw_editor(SDL_Window* window)
	{
		//axis lines
		renderer->draw_line(Vec3f{0, 0, 0}, Vec3f{100, 0, 0}, Vec4f{1, 0, 0, 1});
		renderer->draw_line(Vec3f{0, 0, 0}, Vec3f{0, 100, 0}, Vec4f{0, 1, 0, 1});




		// editor input section
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Uint_32 cam = entity_management_system->get_active_camera();
		Uint_32 cam_transform = camera_system.get_component(cam).id_transform;// cameras[cam].id_transform;
		//mouse_pos_ws = cameras[cam].get_screen_to_world_pos(input_manager->mouse.position);
		//mouse_pos_vs = Vec2f{ clamp<Float_32>(input_manager->mouse.position.x - v_x, 0, v_w) ,
		//	clamp<Float_32>(renderer->height - (input_manager->mouse.position.y - v_y), 0, v_h) };

		// narrowing shouldn't cause problem in screen space (limited space)
		Camera& camera = camera_system.get_component(cam);
		mouse_pos_ss = camera.get_mouse_to_screen_pos(input_manager->mouse.position);
		mouse_pos_ws = (Vec2f)(camera.get_screen_to_world_pos(mouse_pos_ss));

		// entity selection by clicking sprite
		static Vec2f selection_offset;
		if (input_manager->mouse.get_mouse_button_down(1))
		{
			if (is_mouse_inside_viewport(mouse_pos_ss))
			{
				Uint_32 clicked_on_graphic = is_mouse_in_any_graphic(mouse_pos_ws);
				if (clicked_on_graphic)
				{
					Uint_32 clicked_on_transform = graphics_system.get_component(clicked_on_graphic).transform_id;// graphics[clicked_on_graphic].transform_id;
					if (selected_transform == clicked_on_transform)	// 
					{
						drag_transform = selected_transform;
						selection_offset = (Vec2f)transform_system.get_component(selected_transform).get_global_position() - mouse_pos_ws;
					}
					else
					{
						do_edit_collider = false;
						edit_collider_id = 0;
						selected_transform = clicked_on_transform;
						drag_transform = 0;
						//collider_being_edited = 0;

					}
				}
				else
				{
					// check if click was inside viewport
					if (inside_collider_edit_area == false)
					{
						selected_transform = 0;
						do_edit_collider = false;
						edit_collider_id = 0;
						//collider_being_edited = 0;
						GizmoLayer::move_gizmo.target_transform = 0;
						drag_transform = 0;
					}
				}
			}
		}
		// draw rect around selected entity
		if (selected_transform)
		{
			const Mat4x4& _transform = transform_system.get_component(selected_transform).transformation;
			Uint_32 entity_id = transform_system.get_entity(selected_transform);
			Uint_32 graphics_id = graphics_system.get_component_id(entity_id);
			Vec3f* vertices = Graphic::vertex_data[graphics_id];

			if (!do_edit_collider)
			{
				Rect<Float_32> rect = points_to_rect(vertices);
				Vec4f color{ 0.8, 0.5, 0, 1 };
				renderer->draw_rect_with_transform(rect, color,_transform);
			}

		}

		//for (int _i = 0; _i < graphics_system.new_id; _i++)
		//{
		//	if (graphics_system.get_entity(_i))
		//	{
		//		Uint_32 t_id = graphics_system.get_component(_i).transform_id;// graphics[it.first].transform_id;
		//		Mat4x4& _transformation = transform_system.get_component(t_id).transformation;
		//		Vec3f* vertices = Graphic::vertex_data[_i];

		//		Rect<Float_32> rect = points_to_rect(vertices);
		//		Vec4f color{ 0.8, 0.5, 0, 1 };
		//		renderer->draw_rect_with_transform(rect, color, _transformation);
		//	}
		//}

		if (drag_transform)
		{
			if (inside_collider_edit_area == false)
			{
				if (input_manager->mouse.get_mouse_button(1))
				{
					Point3d drag_transform_pos = transform_system.get_component(drag_transform).get_global_position();
					drag_transform_pos.x = mouse_pos_ws.x + selection_offset.x;
					drag_transform_pos.y = mouse_pos_ws.y + selection_offset.y;
					transform_system.get_component(drag_transform).set_global_position(drag_transform_pos);
				}
				else
					drag_transform = 0;
			}
		}

		if (edit_collider_id)
		{
			if (input_manager->mouse.get_mouse_button(1))
			{
				Collider& col = PhysicsModule::collider_system.get_component(edit_collider_id);
				const Mat4x4& trans_inv = transform_system.get_component(col.transform_id).transformation.GetInverse();

				if (edge_id[0]) // top
				{
					col.points[0].y = (trans_inv * mouse_pos_ws).y;
					col.points[1].y = (trans_inv * mouse_pos_ws).y;
				}
				else if (edge_id[1])
				{
					col.points[1].x = (trans_inv * mouse_pos_ws).x;
					col.points[2].x = (trans_inv * mouse_pos_ws).x;

				}
				else if (edge_id[2])
				{
					col.points[2].y = (trans_inv * mouse_pos_ws).y;
					col.points[3].y = (trans_inv * mouse_pos_ws).y;

				}
				else if (edge_id[3])
				{
					col.points[3].x = (trans_inv * mouse_pos_ws).x;
					col.points[0].x = (trans_inv * mouse_pos_ws).x;
				}
			}
			
			if (input_manager->mouse.get_mouse_button_up(1))
			{
				std::memset(edge_id, 0, sizeof(Bool_8) * 4);
				edit_collider_id = 0;
			}
		}
		
		
		Float_32 cam_pan_min_speed = 4.f;
		Float_32 cam_pan_max_speed = 10.f;
		Float_32 cam_pan_speed = cam_pan_min_speed;
		if (input_manager->keyboard.get_key(SDLK_LSHIFT))
			cam_pan_speed = cam_pan_max_speed;


		if (!rename_transform)
		{
			Point3d pos = transform_system.get_component(cam_transform).get_local_position();
			if (input_manager->keyboard.get_key(SDLK_w))
				pos.y = pos.y + (Time::Frame_time*cam_pan_speed);
			if (input_manager->keyboard.get_key(SDLK_s))
				pos.y = pos.y - (Time::Frame_time*cam_pan_speed);
			if (input_manager->keyboard.get_key(SDLK_d))
				pos.x = pos.x + (Time::Frame_time*cam_pan_speed);
			if (input_manager->keyboard.get_key(SDLK_a))
				pos.x = pos.x - (Time::Frame_time*cam_pan_speed);
			transform_system.get_component(cam_transform).set_local_position(pos);
		}

		if (input_manager->mouse.scroll != 0 && is_mouse_inside_viewport(mouse_pos_ss))
		{
			Float_32 speed = 20.f;
			camera.zoom = clamp<Float_32>(camera.zoom + +(input_manager->mouse.scroll*Time::Frame_time*speed), 0.1, 1000);
		}

		if (input_manager->keyboard.get_key_down(SDLK_DELETE))
		{
			entity_management_system->delete_entity_by_transform(selected_transform);
			selected_transform = 0;
			drag_transform = 0;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		draw_scene_hierarchy();
		draw_inspector_window();
		draw_asset_window();
		//ImGui::ShowDemoWindow();
		draw_debug_window(ImGui::GetIO().Framerate);

	}

	void EditorLayer::draw_inspector_window()
	{
		// flags and data for menubar
		static Bool_8 call_add_script = false;
		//static std::string script_to_add = ""; 
		static Uint_32 script_to_add = 0;

		// menubar actions
		if (call_add_script)
		{
			//add_script(script_to_call);
			Script* script = Scripting::get_script_instance(script_names[script_to_add].c_str());
			Uint_32 slctd_tr_entity = transform_system.get_entity(selected_transform);
			entity_management_system->add_script_to_entity(slctd_tr_entity, script, script_to_add);
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
				Uint_32 selected_entity = transform_system.get_entity(selected_transform);
				if (ImGui::BeginMenu("Add..."))
				{
					if (ImGui::MenuItem("Graphics", NULL))
					{
						if (!graphics_system.get_component_id(selected_entity))
						{
							Uint_32 graphic_id = graphics_system.make(selected_entity);
							if (graphic_id)
							{
								Graphic& graphic = graphics_system.get_component(graphic_id);
								graphic.transform_id = selected_transform;
								graphic.tag = RENDER_DYNAMIC;
								graphic.element.material = Material::load_material("Materials/Default.mat", false);
								LOG(LOGTYPE_GENERAL, std::to_string(graphic.element.material));
							}
						}
					}
					if (ImGui::MenuItem("Animator", NULL))
					{
						if (!animator_system.get_component_id(selected_entity))
						{
							if (!graphics_system.get_component_id(selected_entity))
							{
								Uint_32 graphic_id = graphics_system.make(selected_entity);
								if (graphic_id)
								{
									Graphic& graphic = graphics_system.get_component(graphic_id);
									graphic.transform_id = selected_transform;
									graphic.tag = RENDER_DYNAMIC;
									graphic.element.material = Material::load_material("Materials/Default.mat", false);
									LOG(LOGTYPE_GENERAL, std::to_string(graphic.element.material));
								}
							}

							Uint_32 animator_id = animator_system.make(selected_entity);
							if (animator_id)
							{
								Graphic::editor_data[graphics_system.get_component_id(selected_entity)].future_tag = RENDER_DYNAMIC;
								Graphic& graphic = graphics_system.get_component(graphics_system.get_component_id(selected_entity));
								graphic.tag = RENDER_DYNAMIC;

								Animator& animator = animator_system.get_component(animator_id);
								animator.id_transform = selected_transform;

							}
						}
					}
					if (ImGui::MenuItem("Collider", NULL))
					{
						if (!PhysicsModule::collider_system.get_component_id(selected_entity))
						{
							Uint_32 collider_id = PhysicsModule::collider_system.make(selected_entity);
							if (collider_id)
							{
								Collider& collider = PhysicsModule::collider_system.get_component(collider_id);
								collider.transform_id = selected_transform;
								collider.graphic_id = graphics_system.get_component_id(selected_entity);

								collider.type = SHAPE_RECT;
								collider.point_count = 4;
								if (collider.graphic_id)
								{
									collider.points[0] = Graphic::vertex_data[collider.graphic_id][0];
									collider.points[1] = Graphic::vertex_data[collider.graphic_id][1];
									collider.points[2] = Graphic::vertex_data[collider.graphic_id][2];
									collider.points[3] = Graphic::vertex_data[collider.graphic_id][3];
								}
								else
								{
									collider.points[0] = { -1,-1 };
									collider.points[1] = { 1,-1 };
									collider.points[2] = { 1, 1 };
									collider.points[3] = { -1, 1 };
								}

								LOG(LOGTYPE_GENERAL, "Collider added");
							}
						}
					}
					if (ImGui::MenuItem("Camera", NULL))
					{
						if (!camera_system.get_component_id(selected_entity))
						{
							Uint_32 cam_id = camera_system.make(selected_entity);
						}
					}
					if (ImGui::MenuItem("Rigidbody 2D", NULL))
					{
						if (!camera_system.get_component_id(selected_entity))
						{
							Uint_32 rigidbody_id= PhysicsModule::rigidbody2d_system.make(selected_entity);
							Rigidbody2D& rigidbody = PhysicsModule::rigidbody2d_system.get_component(rigidbody_id);
							rigidbody.transform_id = transform_system.get_component_id(selected_entity);
						}
					}

					if (ImGui::BeginMenu("Script"))
					{
						for (Uint_32 _i = 1; _i < script_names.size(); _i++)
						{
							std::string& script_name = script_names[_i];
							if(ImGui::MenuItem(script_name.c_str(),0, &call_add_script))
								script_to_add = _i;
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
			Uint_32 entity = transform_system.get_entity(selected_transform);
			std::string header = "Entity # " + std::to_string(entity);
			ImGui::Text(header.c_str());
			ImGui::NewLine();

			std::string transform_header = "Transform # " + std::to_string(selected_transform);
			if (ImGui::CollapsingHeader(transform_header.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				Point3d pos = transform_system.get_component(selected_transform).get_local_position();// .get_global_position();
				float v3_p[3] = { pos.x, pos.y, pos.z };
				//ImGui::InputFloat("input float", &pos.x, 0.01f, 1.0f, "%.3f");
				ImGui::Text("Position:");
				ImGui::SameLine();
				ImGui::PushID(&pos);
				ImGui::DragFloat3("", v3_p, 1.0f, -10000.0f, 10000.0f, "%.3f", 1.0f);
				ImGui::PopID();
				pos.x = v3_p[0];
				pos.y = v3_p[1];
				pos.z = v3_p[2];
				transform_system.get_component(selected_transform).set_local_position(pos);
				//transforms[selected_transform].position = transforms[selected_transform].get_global_to_local_position(pos);

				Vec3f rot = Quaternion::QuaternionToEuler(transform_system.get_component(selected_transform).get_local_rotation());// transforms[selected_transform].get_local_rotation();// get_global_rotation();
				float v3_r[3] = { rot.x, rot.y, rot.z };
				//ImGui::InputFloat("input float", &pos.x, 0.01f, 1.0f, "%.3f");
				ImGui::Text("Rotation:");
				ImGui::SameLine();
				ImGui::PushID(&rot);
				ImGui::DragFloat3("", v3_r, 1.0f, -10000.0f, 10000.0f, "%.3f", 1.0f);
				rot.x = v3_r[0];
				rot.y = v3_r[1];
				rot.z = v3_r[2];
				ImGui::PopID();
				transform_system.get_component(selected_transform).set_local_rotation(rot);
				//transform_editor_data[selected_transform] = rot;
				//transforms[selected_transform].rotation = transforms[selected_transform].get_global_to_local_rotation(rot);

				Vec3f scl = transform_system.get_component(selected_transform).get_local_scale();
				float v3_s[3] = { scl.x, scl.y, scl.z };
				//ImGui::InputFloat("input float", &pos.x, 0.01f, 1.0f, "%.3f");
				ImGui::Text("Scale:   ");
				ImGui::SameLine();
				ImGui::PushID(&scl);
				ImGui::DragFloat3("", v3_s, 1.0f, -10000.0f, 10000.0f, "%.3f", 1.0f);
				ImGui::PopID();

				scl.x = v3_s[0];
				scl.y = v3_s[1];
				scl.z = v3_s[2];
				transform_system.get_component(selected_transform).set_local_scale(scl);

			}
			last_selected_transform = selected_transform;

			//auto& ent = entities[entity];
			if (graphics_system.get_component_id(entity))
			{
				Uint_32 graphic_id = graphics_system.get_component_id(entity);// ent[COMP_GRAPHICS];
				std::string graphics_header = "Graphics # " + std::to_string(graphic_id);
				if (ImGui::CollapsingHeader(graphics_header.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					Graphic& g = graphics_system.get_component(graphic_id);// [graphic_id];
					//static Int_32 _tag = (Int_32)g.tag;
					ImGui::Combo("Render Mode", &Graphic::editor_data[graphic_id].future_tag, "Untagged\0Static\0Dynamic\0", 3);
					//g.tag = (RenderTag)_tag;

					ImGui::SetNextItemOpen(true);
					if (ImGui::TreeNode("Material"))
					{
						Uint_32 mat_id = g.element.material;
						std::string& mat_name = Material::material_names[mat_id];
						ImGui::SetNextItemOpen(true);
						if (ImGui::TreeNode(mat_name.c_str()))
						{
							if (ImGui::BeginDragDropTarget())
							{
								const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Material");
									if (payload)
									{
										IM_ASSERT(payload->DataSize == sizeof(Int_32));
											auto p = material_directories[*(const Int_32*)payload->Data];
											Uint_32 mat_id = Material::load_material(p, false);
											if (mat_id)
												g.element.material = mat_id;// *(const int*)payload->Data;
											else
												LOG(LOGTYPE_ERROR, "material couldn't be created");
									}
								ImGui::EndDragDropTarget();

							}

							ImGui::BeginChild("Textures", ImVec2(0, 8 * 25), true, 0);
							Material* material = Material::get_material(mat_id);
							for (Uint_32 _tex_ind = 0; _tex_ind < 8; _tex_ind++)
							{
								//no_of_animations++;
								Uint_32 tex_id = material->diffuse_textures[_tex_ind];
								if (tex_id != -1)
								{
									std::string& _texture_name = Texture::texture_names[tex_id];
									std::string _node_label = _texture_name;
									ImGui::Text(_node_label.c_str());
									if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
									{
										ImGui::SetDragDropPayload("Texture", &tex_id, sizeof(Uint_32));
										ImGui::EndDragDropSource();
									}
								}
								else
									ImGui::Text("...");

								if (ImGui::BeginDragDropTarget())
								{
									const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture");
									if (payload)
									{
										IM_ASSERT(payload->DataSize == sizeof(Uint_32));
										Uint_32 t_id = *(Uint_32*)payload->Data;
										LOG(LOGTYPE_GENERAL, "Assigned texture : ", std::to_string(t_id));
										material->diffuse_textures[_tex_ind] = t_id;// *(const int*)payload->Data;
									}
									ImGui::EndDragDropTarget();
								}
							}
							ImGui::EndChild();

							ImGui::TreePop();

						}
						ImGui::TreePop();
					}
				}
			}

			if (PhysicsModule::rigidbody2d_system.get_component_id(entity))
			{
				Uint_32 rigidbody_id = PhysicsModule::rigidbody2d_system.get_component_id(entity);
				std::string rigidbody_header = "Rigidbody2D # " + std::to_string(rigidbody_id);

				if (ImGui::CollapsingHeader(rigidbody_header.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					Rigidbody2D& rigidbody2d = PhysicsModule::rigidbody2d_system.get_component(rigidbody_id);
					ImGui::Text("Mass");
					ImGui::SameLine();
					ImGui::PushID(&rigidbody2d.mass_inverse);
					Float_32 mass = 1.f / rigidbody2d.mass_inverse;;
					ImGui::PushItemWidth(-0.0001f);
					ImGui::DragFloat("", &mass, 0.001f, 0.000001f, 1000, "%.6f");
					ImGui::PopItemWidth();
					rigidbody2d.mass_inverse = 1.f / mass;
					ImGui::PopID();

					ImGui::NewLine();

					ImGui::Text("Drag");
					ImGui::SameLine();
					ImGui::PushID(&rigidbody2d.drag);
					ImGui::PushItemWidth(-0.0001f);
					ImGui::DragFloat("", &rigidbody2d.drag, 0.001f, 0.f, 100.f, "%.3f");
					ImGui::PopID();

					ImGui::NewLine();

					ImGui::Text("Angular drag");
					ImGui::SameLine();
					ImGui::PushID(&rigidbody2d.angular_drag);
					ImGui::PushItemWidth(-0.0001f);
					ImGui::DragFloat("", &rigidbody2d.angular_drag, 0.001f, 0.f, 100.f, "%.3f");
					ImGui::PopID();

					ImGui::NewLine();

					ImGui::Text("Velocity");
					ImGui::SameLine();
					ImGui::PushID(&rigidbody2d.velocity);
					ImGui::PushItemWidth(-0.0001f);
					ImGui::DragFloat2("", (Float_32*)(&rigidbody2d.velocity), 0.001f, -100.f, 100.f);
					ImGui::PopItemWidth();
					ImGui::PopID();

					ImGui::NewLine();

					ImGui::Text("Acceleration");
					ImGui::SameLine();
					ImGui::PushID(&rigidbody2d.acceleration);
					ImGui::PushItemWidth(-0.0001f);
					ImGui::DragFloat2("", (Float_32*)(&rigidbody2d.acceleration), 0.001f, -100.f, 100.f);
					ImGui::PopItemWidth();
					ImGui::PopID();

					ImGui::NewLine();

					ImGui::Text("Angular Velocity");
					ImGui::SameLine();
					ImGui::PushID(&rigidbody2d.angular_velocity);
					ImGui::PushItemWidth(-0.0001f);
					ImGui::DragFloat("", &rigidbody2d.angular_velocity, 0.001f, -100.f, 100.f);
					ImGui::PopItemWidth();
					ImGui::PopID();

					ImGui::NewLine();

					ImGui::Text("Angular Acceleration");
					ImGui::SameLine();
					ImGui::PushID(&rigidbody2d.angular_acceleration);
					ImGui::PushItemWidth(-0.0001f);
					ImGui::DragFloat("", &rigidbody2d.angular_acceleration, 0.001f, -100.f, 100.f);
					ImGui::PopItemWidth();
					ImGui::PopID();


					ImGui::NewLine();

					ImGui::Text("Kinematic");
					ImGui::SameLine();
					ImGui::PushID(&rigidbody2d.is_kinematic);
					ImGui::PushItemWidth(-0.0001f);
					ImGui::Checkbox("", &rigidbody2d.is_kinematic);
					ImGui::PopItemWidth();
					ImGui::PopID();
					
					ImGui::NewLine();

					ImGui::Text("Coefficient of Restitution");
					ImGui::SameLine();
					ImGui::PushID(&rigidbody2d.coefficient_of_restitution);
					ImGui::DragFloat("", &rigidbody2d.coefficient_of_restitution, 0.01f, 0.0f, 1.0f);
					ImGui::PopID();
					
					ImGui::NewLine();

					ImGui::Text("Static Friction");
					ImGui::SameLine();
					ImGui::PushID(&rigidbody2d.static_friction);
					ImGui::DragFloat("", &rigidbody2d.static_friction, 0.01f, 0.0f, 1.0f);
					ImGui::PopID();

					ImGui::NewLine();

					ImGui::Text("Dynamic Friction");
					ImGui::SameLine();
					ImGui::PushID(&rigidbody2d.dynamic_friction);
					ImGui::DragFloat("", &rigidbody2d.dynamic_friction, 0.01f, 0.0f, 1.0f);
					ImGui::PopID();

					ImGui::NewLine();

					ImGui::Text("Rotation");
					ImGui::SameLine();
					ImGui::PushID(&rigidbody2d.rotation);
					ImGui::DragFloat("", &rigidbody2d.rotation, 1.f);
					ImGui::PopID();



				}
			}

			Uint_32 col_id = PhysicsModule::collider_system.get_component_id(entity);
			//edit_collider_id = 0;
			if (col_id)
			{
				std::string collider_header = "Collider # " + std::to_string(col_id);

				if (ImGui::CollapsingHeader(collider_header.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					Collider& collider = PhysicsModule::collider_system.get_component(col_id);
					

					static Vec4f collider_color = { 0.1f, 0.5f, 0.3f, 1.f };
					const Transform3D& collider_transform = transform_system.get_component(selected_transform);
					const Mat4x4& collider_transformation = collider_transform.transformation;



					const char* items[] = { "Point", "Circle", "Rect", "Polygon", "AABB" };
					int item_current = collider.type;
					ImGui::Text("Collider type");
					ImGui::PushID(&item_current);
					ImGui::Combo("", &item_current, items, IM_ARRAYSIZE(items));
					ImGui::PopID();
					ImGui::NewLine();
					collider.type = (Shape2DTypes)item_current;

					ImGui::Text("Edit Edges");
					ImGui::SameLine();
					ImGui::PushID(&collider);
					ImGui::Checkbox("", &do_edit_collider);
					ImGui::PopID();
					ImGui::NewLine();



					if(collider.type == SHAPE_POLY)
					{
						renderer->draw_shape_with_transform(collider.points, collider.point_count, collider_transformation, collider_color);
					}

					if (do_edit_collider)
					{
						switch ((Shape2DTypes)item_current)
						{
						case SHAPE_RECT:
						{
							static Float_32 col_width = collider.points[0].x - collider.points[1].x;
							static Float_32 col_height = collider.points[0].y - collider.points[3].y;

							ImGui::Text("Width");
							ImGui::SameLine();
							ImGui::PushID(&col_width);
							ImGui::DragFloat("", &col_width, 0.01f);
							ImGui::PopID();

							ImGui::NewLine();

							ImGui::Text("Height");
							ImGui::SameLine();
							ImGui::PushID(&col_height);
							ImGui::DragFloat("", &col_height, 0.01f);
							ImGui::PopID();

							Float_32 cur_w = collider.points[0].x - collider.points[1].x;
							Float_32 cur_h = collider.points[0].y - collider.points[3].y;

							Float_32 _x_h_dif = (col_width - cur_w) / 2.f;
							Float_32 _y_h_dif = (col_height - cur_h) / 2.f;

							collider.points[0].x += _x_h_dif;
							collider.points[3].x += _x_h_dif;
							collider.points[1].x -= _x_h_dif;
							collider.points[2].x -= _x_h_dif;

							collider.points[0].y += _y_h_dif;
							collider.points[1].y += _y_h_dif;
							collider.points[2].y -= _y_h_dif;
							collider.points[3].y -= _y_h_dif;

							/*renderer->draw_line_with_transform(collider.points[0], collider.points[1], colilder_color, selected_transformation);
							renderer->draw_line_with_transform(collider.points[1], collider.points[2], colilder_color, selected_transformation);
							renderer->draw_line_with_transform(collider.points[2], collider.points[3], colilder_color, selected_transformation);
							renderer->draw_line_with_transform(collider.points[3], collider.points[0], colilder_color, selected_transformation);*/

							Rect<Float_32> _rect = points_to_rect(collider.points);
							renderer->draw_rect_with_transform(_rect, collider_color, collider_transformation);
						}
						break;

						case SHAPE_CIRCLE:
						{
							if (do_edit_collider)
							{
								ImGui::Text("Radius");
								ImGui::SameLine();
								ImGui::PushID(&collider.points[0].x);
								ImGui::DragFloat("", &collider.points[0].x, 0.01f);
								ImGui::PopID();

								Vec2f origin = collider_transform.get_global_position();
								renderer->draw_circle(origin, collider.points[0].x, collider_color);
							}
						}
						break;

						case SHAPE_POLY:
						{
							renderer->draw_shape_with_transform(collider.points, collider.point_count, collider_transformation, collider_color);
						}
						break;
						}
					}


					

					

					
					

					//const Mat4x4& selected_transformation = transform_system.get_component(selected_transform).transformation;
					//Vec2f points[4];
					//std::memcpy(points, collider.points, sizeof(Vec2f) * 4);
					//points[0] *= 1.3f;
					//points[1] *= 1.3f;
					//points[2] *= 1.3f;
					//points[3] *= 1.3f;
					//Rect<Float_32> collision_area_rect = points_to_rect(points, selected_transformation);
					//Rect<Float_32> collision_rect = points_to_rect(collider.points, selected_transformation);
					//

					////renderer->draw_rect_with_transform(collision_rect, { 0,0.8f,0.2f,1 }, selected_transformation);
					////Vec4f collider_color = { 0, 0.5f, 0.5f, 1.f };
					////Vec4f collider_color_near_line = { 0, 1.f, 0.f, 1.f };
					//Vec4f top_color = { 0.1f, 0.5f, 0.3f, 1.f };
					//Vec4f left_color = { 0.1f, 0.5f, 0.3f, 1.f };
					//Vec4f bottom_color = { 0.1f, 0.5f, 0.3f, 1.f };
					//Vec4f right_color = { 0.1f, 0.5f, 0.3f, 1.f };


					////if (collider.transform_id == selected_)
					//if (do_edit_collider)
					//{
					//	if (0)//(point_in_AABB(mouse_pos_ws, collision_area_rect))
					//	{
					//		if (abs<Float_32>(collision_rect.y + collision_rect.h - mouse_pos_ws.y) <= 0.2f)
					//		{
					//			top_color = { 0, 1.f, 0.f, 1.f };
					//			if (input_manager->mouse.get_mouse_button_down(1))
					//			{
					//				edge_id[0] = 1;
					//				edit_collider_id = col_id;
					//			}
					//		}
					//		else if (abs<Float_32>(collision_rect.x - mouse_pos_ws.x) <= 0.2f)
					//		{
					//			left_color = { 0, 1.f, 0.f, 1.f };
					//			if (input_manager->mouse.get_mouse_button_down(1))
					//			{
					//				edge_id[1] = 1;
					//				edit_collider_id = col_id;
					//			}
					//		}
					//		else if (abs<Float_32>(collision_rect.y - mouse_pos_ws.y) <= 0.2f)
					//		{
					//			bottom_color = { 0, 1.f, 0.f, 1.f };
					//			if (input_manager->mouse.get_mouse_button_down(1))
					//			{
					//				edge_id[2] = 1;
					//				edit_collider_id = col_id;
					//			}
					//		}
					//		else if (abs<Float_32>(collision_rect.x + collision_rect.w - mouse_pos_ws.x) <= 0.2f)
					//		{
					//			right_color = { 0, 1.f, 0.f, 1.f };
					//			if (input_manager->mouse.get_mouse_button_down(1))
					//			{
					//				edge_id[3] = 1;
					//				edit_collider_id = col_id;
					//			}
					//		}
					//	}

					//	renderer->draw_line_with_transform(collider.points[0], collider.points[1], top_color, selected_transformation);
					//	renderer->draw_line_with_transform(collider.points[1], collider.points[2], left_color, selected_transformation);
					//	renderer->draw_line_with_transform(collider.points[2], collider.points[3], bottom_color, selected_transformation);
					//	renderer->draw_line_with_transform(collider.points[3], collider.points[0], right_color, selected_transformation);

					//	inside_collider_edit_area = true;
					//}
					//else
					//	inside_collider_edit_area = false;
					
				}


			}

			if (camera_system.get_component_id(entity))
			{
				Uint_32 camera_id = entity_management_system->get_active_camera();
				std::string camera_header = "Camera # " + std::to_string(camera_id);

				if (ImGui::CollapsingHeader(camera_header.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					Camera& camera = camera_system.get_component(camera_id);

					auto avail_width = ImGui::GetContentRegionAvail().x;


					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::Text("Left  ");
					ImGui::PopItemWidth();

					ImGui::SameLine();

					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::PushID(&camera.left);
					ImGui::DragFloat("", &camera.left, -0.5f, -1000.f, 1000.f, "%.2f", 1.f);
					ImGui::PopItemWidth();
					ImGui::PopID();

					ImGui::SameLine();

					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::Text("Right ");
					ImGui::PopItemWidth();

					ImGui::SameLine();
					
					ImGui::PushID(&camera.right);
					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::DragFloat("", &camera.right, -0.5f, -1000.f, 1000.f, "%.2f", 1.f);
					ImGui::PopItemWidth();
					ImGui::PopID();

					ImGui::NewLine();

					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::Text("Top   ");
					ImGui::PopItemWidth();
					
					ImGui::SameLine();

					ImGui::PushID(&camera.top);
					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::DragFloat("", &camera.top, -0.5f, -1000.f, 1000.f, "%.2f", 1.f);
					ImGui::PopItemWidth();
					ImGui::PopID();

					ImGui::SameLine();

					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::Text("Bottom");
					ImGui::PopItemWidth();

					ImGui::SameLine();

					ImGui::PushID(&camera.bottom);
					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::DragFloat("", &camera.bottom, -0.5f, -1000.f, 1000.f, "%.2f", 1.f);
					ImGui::PopItemWidth();
					ImGui::PopID();

					ImGui::NewLine();

					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::Text("Near  ");
					ImGui::PopItemWidth();

					ImGui::SameLine();
					
					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::PushID(&camera.near_);
					ImGui::DragFloat("", &camera.near_, -0.5f, -1000.f, 1000.f, "%.2f", 1.f);
					ImGui::PopItemWidth();
					ImGui::PopID();

					ImGui::SameLine();

					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::Text("Top   ");
					ImGui::PopItemWidth();

					ImGui::SameLine();

					ImGui::PushID(&camera.far_);
					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::DragFloat("", &camera.far_, -0.5f, -1000.f, 1000.f, "%.2f", 1.f);
					ImGui::PopItemWidth();
					ImGui::PopID();

					ImGui::NewLine();

					ImGui::PushItemWidth(avail_width*0.25f);
					ImGui::Text("Zoom Out");
					ImGui::PopItemWidth();

					ImGui::SameLine();

					ImGui::PushID(&camera.zoom);
					ImGui::PushItemWidth(-FLT_MIN);
					ImGui::DragFloat("", &camera.zoom, 0.01f, 0.f, 1000.f, "%.2f", 1.f);
					ImGui::PopItemWidth();
					ImGui::PopID();

				}
			}

			if (animator_system.get_component_id(entity))
			{
				Uint_32 id_animator = animator_system.get_component_id(entity);// ent[COMP_ANIMATOR];
				std::string animator_header = "Animator # " + std::to_string(id_animator);

				if (ImGui::CollapsingHeader(animator_header.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{

					Animator& animator = animator_system.get_component(id_animator);// animators[id_animator];
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

					ImGui::BeginChild("Animations",ImVec2(0, animator.animation_count*25),true,0);

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
								Uint_32 g_id = graphics_system.get_component_id(transform_system.get_entity(selected_transform));
								Graphic& graphic = graphics_system.get_component(g_id);
								graphic.animator_id = id_animator;
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

			if (scripting_system.get_component_id(entity))
			{
				Uint_32 scripting_id = scripting_system.get_component_id(entity);
				Scripting& sc = scripting_system.get_component(scripting_id);// scripting_comps[ent[COMP_SCRIPT]];
				for (auto& ref_id : sc.ref_table)
				{
					Script* _s = sc.get_script(ref_id.first);
					if (_s)
					{
						const char* _n = sc.get_script_name(ref_id.first);
						ImGui::PushID(ref_id.first);
						if (ImGui::CollapsingHeader(_n, ImGuiTreeNodeFlags_DefaultOpen))
						{
							if (ImGui::Button("Remove..."))
							{
								sc.remove_script(ref_id.first);
							}
							ImGui::Separator();
						}
						ImGui::PopID();
					}

				}

			}
		}

		ImGui::End();
	}

	void EditorLayer::draw_scene_hierarchy()
	{
		//if (!ImGui::Begin("Scene Hierarchy", 0, ImGuiWindowFlags_NoMove))
		ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
		if (!ImGui::Begin("Scene Hierarchy", 0, flags))
		{
			ImGui::End();
			return;
		}


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Add..."))
			{
				if (ImGui::MenuItem("Entity"))
				{
					Uint_32 new_entity = entity_management_system->make_entity("New Entity");
					Uint_32 transform_id = transform_system.make(new_entity);
					if (!transform_id)
						LOG(LOGTYPE_ERROR, "Failed to creae new transform");
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		//v_x = ImGui::GetWindowPos().x + ImGui::GetWindowWidth();

		//Uint_32 max_hierarchy = 0;
		for (int _i = 1; _i < transform_system.new_id; _i++)
		{
			if (transform_system.get_entity(_i))
			{
				if (transform_system.get_component(_i).parent_transform == 0)// max_hierarchy)
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
			Material::load_material(p, false);
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
		get_files_in_dir(dir, ".gif", texture_directories);
		for (int i = 0; i < texture_directories.size(); i++)
		{
			auto p = texture_directories[i].substr(dir.size());
			texture_directories[i] = p;

			Uint_32 tex_id = Texture::load_texture(p, false);
			EntityGenerator::batched_texture_ids.push_back(tex_id);
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

	void EditorLayer::draw_asset_window()
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
			for (Uint_32 j = 0; j < texture_directories.size(); j++)
			{
				if (ImGui::Selectable(texture_directories[j].c_str(), selected_col_2 == j))
				{
					selected_col_2 = j;
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					Uint_32 t_id = Texture::load_texture(texture_directories[j], false);
					ImGui::SetDragDropPayload("Texture", &t_id, sizeof(Uint_32));
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
		for (int _i=0; _i<graphics_system.new_id; _i++)
		{
			if (graphics_system.get_entity(_i))
			{
				Uint_32 t_id = graphics_system.get_component(_i).transform_id;// graphics[it.first].transform_id;
				Vec2f points[4];
				Mat4x4& _transformation = transform_system.get_component(t_id).transformation;
				Vec3f* vertices = Graphic::vertex_data[_i];
				points[0] = _transformation * vertices[0];
				points[1] = _transformation * vertices[1];
				points[2] = _transformation * vertices[2];
				points[3] = _transformation * vertices[3];

				if (point_in_shape(points, 4, mouse_pos))
					return _i;
			}
		}

		return 0;
	}


	void EditorLayer::add_script(Uint_32 entity, std::string& script_name)
	{
	}

	void EditorLayer::draw_debug_window(Float_32 fps)
	{
		if (!ImGui::Begin("Info: ", 0))//, ImGuiWindowFlags_NoMove))
		{
			ImGui::End();
			return;
		}

		/*ImVec2 _pos = ImGui::GetWindowPos();
		ImVec2 _size = ImGui::GetWindowSize();
		_pos.x = v_x + v_w - _size.x;
		ImGui::SetWindowPos(_pos);*/

		ImGui::Text(("(WS): " + std::to_string(mouse_pos_ws.x)+","+ std::to_string(mouse_pos_ws.y)).c_str());
		ImGui::Text(("(SS): " + std::to_string(mouse_pos_ss.x)+","+ std::to_string(mouse_pos_ss.y)).c_str());
		ImGui::Text("(%.1f FPS)", fps);
		
		ImGui::End();
	}

}