#vertex
#version 330 core
layout(location=0) in vec4 position;
layout(location=1) in vec4 _color;
layout(location=2) in vec4 _normal;
layout(location=3) in vec2 texco;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * position;
}

#fragment
#version 330 core

out vec4 color;
void main()
{
    color = vec4(1,1,1,1);
}
