#vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 _color;
layout(location = 2) in vec4 _normal;
layout(location = 3) in vec2 texco;
layout(location = 4) in float mat_id;

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
out float out_mat_id;
//out float out_light;
out vec2 out_tiling;
out vec2 out_panning;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * position;
	world_pos = vec3(u_Model * position);
	out_color = _color;
	out_mat_id = mat_id;
	out_diffuse_color = u_Diffuse_Color;
	out_outline_color = u_Outline_Color;
	out_texco = texco;
	out_tiling = u_Tiling;
	out_panning = u_Panning;
}

#fragment
#version 330 core

//uniform float u_red;
uniform sampler2D u_textures[8];
uniform float u_Ambient_Strength;
uniform vec3 u_Camera_Position;

in vec4 out_color;
in vec3 out_diffuse_color;
in vec2 out_texco;
in vec3 world_pos;
in vec2 out_tiling;
in vec2 out_panning;
in vec4 out_outline_color;
in float out_mat_id;
out vec4 color;

void main()
{
	vec2 pan_tile = (out_texco.xy + out_panning.xy)*out_tiling.xy;
	int _ind = int(out_mat_id);
	vec4 _color = texture(u_textures[_ind], pan_tile) * out_color * vec4(out_diffuse_color, 1) * u_Ambient_Strength;// * out_light;// vec4(u_red, out_color.gba);//vec4(0.0,1.0,1.0,1.0);
	if (_color.a < 0.5)
		discard;
	else
		color = _color;
}
