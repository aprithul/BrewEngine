#vertex

#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec4 _color;
layout(location=2) in vec4 _normal;
layout(location=3) in vec2 texco;

uniform mat4 u_Model;
uniform mat4 u_View_t;
uniform mat4 u_Projection;

out vec3 out_texco;

void main()
{
    out_texco = vec3(position);
    gl_Position = u_Projection * u_View_t * u_Model * position;
}

#fragment
#version 330 core

//uniform float u_red;
uniform samplerCube u_skybox;
in vec3 out_texco;

out vec4 color;
void main()
{
    color = texture(u_skybox, out_texco);    
}
