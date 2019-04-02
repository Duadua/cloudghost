#version 330 core

const float magnitude = 0.4;

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
} i_gs[];

out O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
} o_gs;

uniform float u_time;

vec3 get_normal() {
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal) {
    vec3 direction = normal * ((sin(u_time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}

void main() {    
    vec3 normal = get_normal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    o_gs.tex_coord = i_gs[0].tex_coord;
    o_gs.world_pos = i_gs[0].world_pos;
    o_gs.normal = i_gs[0].normal;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    o_gs.tex_coord = i_gs[1].tex_coord;
    o_gs.world_pos = i_gs[1].world_pos;
    o_gs.normal = i_gs[1].normal;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    o_gs.tex_coord = i_gs[2].tex_coord;
    o_gs.world_pos = i_gs[1].world_pos;
    o_gs.normal = i_gs[2].normal;
    EmitVertex();

    EndPrimitive();
}



