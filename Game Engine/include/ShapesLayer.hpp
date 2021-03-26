#ifndef  SHAPES_LAYER_HPP
#define SHAPES_LAYER_HPP

#include "RenderLayer.hpp"

namespace PrEngine {
	struct ShapesLayer : public RenderLayer
	{
		void start() override;
		void update() override;
		void end() override;
	};
}

#endif // ! SHAPE_LAYER_HPP
