#include "GizmoLayer.hpp"
#include "RendererOpenGL2D.hpp"

namespace PrEngine {

	MoveGizmo GizmoLayer::move_gizmo;


	Material* move_mat;

	void GizmoLayer::start()
	{
		Uint_32 mat_id = Material::load_material("Materials" + PATH_SEP + "Gizmo.mat", true);
		move_mat = Material::get_material(mat_id);
		move_gizmo.graphic_x.element.material = mat_id;
		move_gizmo.graphic_y.element.material = mat_id;
		renderer->generate_sprite_graphics(move_gizmo.graphic_x);
		renderer->generate_sprite_graphics(move_gizmo.graphic_y);

		move_gizmo.transform.set_local_scale(0.5, 0.5, 0.5);
			
	}

	void GizmoLayer::update()
	{
		// update transforms
		move_gizmo.transform.update();
		

		// get camera
		Camera& _camera = camera_system.get_component(entity_management_system->get_active_camera());// cameras[entity_management_system->get_active_camera()];
		// get light
		//DirectionalLight& _light = directional_lights[1];

		if (move_gizmo.target_transform)
		{
			Transform3D& target_transform = transform_system.get_component(move_gizmo.target_transform);
			move_gizmo.transform.set_local_position(target_transform.get_local_position().x, target_transform.get_local_position().y, 1);

			//draw gizmos
			move_mat->diffuse_color = Vec4f(1, 0.26f, 0.26f, 1); // red
			renderer->render_graphic(move_gizmo.graphic_x, move_gizmo.transform.transformation, _camera);

			move_gizmo.transform.Rotate(0, 0, 90);
			move_gizmo.transform.update();
			move_mat->diffuse_color = Vec4f(0.26f, 1, 0.52f, 1); // green
			renderer->render_graphic(move_gizmo.graphic_y, move_gizmo.transform.transformation, _camera);
			move_gizmo.transform.Rotate(0, 0, -90);
			move_gizmo.transform.update();

		}
	}

	void GizmoLayer::end()
	{

	}
}