#ifndef  SHAPES_LAYER_HPP
#define SHAPES_LAYER_HPP

#include "RenderLayer.hpp"

namespace PrEngine {


	struct Line
	{
		Vec3f p1;
		Vec3f p2;
		Vec4f color;
	};
	extern std::vector<Line> lines;


	struct ShapesLayer : public RenderLayer
	{
		void start() override;
		void update() override;
		void end() override;
	};
}

#endif // ! SHAPE_LAYER_HPP
