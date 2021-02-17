#vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 _color;
layout(location = 2) in vec4 _normal;
layout(location = 3) in vec2 texco;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 v_color;

void main()
{
	gl_Position = u_Projection * u_View * position;
	v_color = _color;
}

#fragment
#version 330 core

in vec4 v_color;

out vec4 color;
void main()
{
	color = v_color;// vec4(1, 1, 1, 1);
}
