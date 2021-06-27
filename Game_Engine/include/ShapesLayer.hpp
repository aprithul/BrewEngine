#ifndef  SHAPES_LAYER_HPP
#define SHAPES_LAYER_HPP

#include "RenderLayer.hpp"

namespace PrEngine {


	struct Line
	{
		Vec4f color;
		Vec3f p1;
		Vec3f p2;
	};

	struct Quad
	{
		Vec4f color;
		Point3d p1;
		Point3d p2;
		Float_32 w;
		Float_32 h;
	};

	extern std::vector<Line> lines;
	extern std::vector<Quad> quads;


	struct ShapesLayer : public RenderLayer
	{
		void start() override;
		void update() override;
		void end() override;
	};
}

#endif // ! SHAPE_LAYER_HPP
