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

uniform vec2 u_Tiling;
uniform vec2 u_Panning;

out vec4 out_color;
out vec2 out_texco;
out vec3 world_pos;
//out float out_light;
out vec2 out_tiling;
out vec2 out_panning;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * position;
    world_pos = vec3(u_Model * position);
    out_color = _color;
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
in vec2 out_texco;
in vec3 world_pos;
in vec2 out_tiling;
in vec2 out_panning;

out vec4 color;
void main()
{
    color = texture(u_sampler2d, (out_texco.xy+out_panning.xy)*out_tiling.xy) * out_color;// * out_light;// vec4(u_red, out_color.gba);//vec4(0.0,1.0,1.0,1.0);
}
