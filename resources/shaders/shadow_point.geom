#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_light_proj_view[6];

out O_GS {
	vec4 world_pos;
} o_gs;

void main() {
    for(int face = 0; face < 6; ++face) {
        gl_Layer = face;					// built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) {
            o_gs.world_pos = gl_in[i].gl_Position;
            gl_Position = u_light_proj_view[face] * o_gs.world_pos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}