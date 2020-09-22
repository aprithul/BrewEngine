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
uniform vec3 u_Dir_Light;
uniform vec2 u_Tiling;
uniform vec2 u_Panning;

out vec4 out_color;
out vec2 out_texco;
out float out_light;
out vec2 out_tiling;
out vec2 out_panning;
out vec4 out_worldpos;
out vec3 out_normal;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * position;
    out_worldpos = u_Model * position;
    out_color = _color;
    out_texco = texco;
    out_normal = normalize( vec3(u_Model *_normal));
    out_light = clamp((1.0 - dot(out_normal, u_Dir_Light)),0,1);
    out_tiling = u_Tiling;
    out_panning = u_Panning;
}

#fragment
#version 330 core

//uniform float u_red;
uniform sampler2D u_sampler2d;
uniform samplerCube u_env_map;
uniform vec3 u_Camera_Position;

in vec4 out_color;
in vec2 out_texco;
in vec4 out_worldpos;
in float out_light;
in vec2 out_tiling;
in vec2 out_panning;
in vec3 out_normal;

out vec4 color;
void main()
{
    vec3 reflection_vec = reflect( normalize(out_worldpos.xyz - u_Camera_Position), out_normal);
    //u_sampler2d;
    //color = vec4(out_light,out_light,out_light,1);// vec4(out_light,out_light,out_light,1.0);// texture(u_sampler2d, out_texco)*2.0;// * out_light;// * out_light;// vec4(u_red, out_color.gba);//vec4(0.0,1.0,1.0,1.0);
    vec4 tex_col = texture(u_sampler2d, (out_texco.xy+out_panning.xy)*out_tiling.xy);// * out_light;// vec4(u_red, out_color.gba);//vec4(0.0,1.0,1.0,1.0);
    vec4 refl_col = texture(u_env_map, reflection_vec);
    //if(tex_col.a <0.7f)
    //    discard;
    //else
    color = mix(tex_col, refl_col, 0.7);
    //color = vec4(tex_col.rgb* out_light, tex_col.a);
        //color = vec4(vec3(gl_FragCoord.z), tex_col.a);
}
