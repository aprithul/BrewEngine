#vertex
#version 320 es
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 _color;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 v_color;

void main()
{
	gl_Position = u_Projection * u_View * position;
	v_color = _color;
}

#fragment
#version 320 es

precision mediump float;

in vec4 v_color;

out vec4 color;
void main()
{
	//vec4 _col = vec4((v_color & 0xff000000u) >> 24, (v_color & 0x00ff0000u) >> 16, (v_color & 0x0000ff00u) >> 8, (v_color & 0x000000ffu));
	color = v_color;// vec4(1, 1, 1, 1);
}
