#vertex
#version 330 core
layout(location=0) in vec4 position;
layout(location=1) in vec4 _color;
layout(location=2) in vec4 _normal;
layout(location=3) in vec2 texco;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Normal_M;
uniform vec3 u_Diffuse_Color;
uniform vec4 u_Outline_Color;
uniform vec2 u_Tiling;
uniform vec2 u_Panning;

out vec4 out_color;
out vec2 out_texco;
out vec3 world_pos;
out vec3 out_diffuse_color;
out vec4 out_outline_color;

//out float out_light;
out vec2 out_tiling;
out vec2 out_panning;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * position;
    world_pos = vec3(u_Model * position);
    out_color = _color;
	out_diffuse_color = u_Diffuse_Color;
	out_outline_color = u_Outline_Color;
    out_texco = texco;
    out_tiling = u_Tiling;
    out_panning = u_Panning;
}

#fragment
#version 330 core

//uniform float u_red;
uniform sampler2D u_sampler2d;
uniform float u_Ambient_Strength;
uniform vec3 u_Camera_Position;

in vec4 out_color;
in vec3 out_diffuse_color;
in vec2 out_texco;
in vec3 world_pos;
in vec2 out_tiling;
in vec2 out_panning;
in vec4 out_outline_color;
out vec4 color;
void main()
{
	vec2 pan_tile = (out_texco.xy + out_panning.xy)*out_tiling.xy;
    vec4 texel = texture(u_sampler2d, pan_tile) * out_color * vec4(out_diffuse_color,1) * u_Ambient_Strength;// * out_light;// vec4(u_red, out_color.gba);//vec4(0.0,1.0,1.0,1.0);
	if (texel.a < 0.5)
	{
		if (out_outline_color.a < 0.5)
			discard;

		vec2 thickness = 1.0 / vec2(textureSize(u_sampler2d, 0)) * 2;

		vec4 top = texture(u_sampler2d, vec2(pan_tile.x, pan_tile.y + thickness.y));
		vec4 bottom = texture(u_sampler2d, vec2(pan_tile.x, pan_tile.y - thickness.y));
		vec4 left = texture(u_sampler2d, vec2(pan_tile.x - thickness.x, pan_tile.y));
		vec4 right = texture(u_sampler2d, vec2(pan_tile.x + thickness.x, pan_tile.y));

		if (top.a > 0.9 || bottom.a > 0.9 || left.a > 0.9 || right.a > 0.9)
			color = out_outline_color;
		else
			discard;

	}
	else
		color = texel;
}
