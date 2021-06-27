#ifndef  GIZMO_LAYER_HPP
#define GIZMO_LAYER_HPP

#include "RenderLayer.hpp"

namespace PrEngine {

	struct MoveGizmo
	{
		Graphic graphic_x;
		Graphic graphic_y;
		Transform3D transform;
		Uint_32 target_transform;
	};


	struct GizmoLayer : public RenderLayer
	{
		// gizmos
		static MoveGizmo move_gizmo;


		void start() override;
		void update() override;
		void end() override;
	};
}

#endif // ! GIZMO_LAYER_HPP
