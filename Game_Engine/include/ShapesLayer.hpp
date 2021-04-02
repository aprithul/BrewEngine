#ifndef  SHAPES_LAYER_HPP
#define SHAPES_LAYER_HPP

#include "RenderLayer.hpp"

namespace PrEngine {


	struct Line
	{
		Vector3<Float_32> p1;
		Vector3<Float_32> p2;
		Vector4<Float_32> color;
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
