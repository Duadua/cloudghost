#version 330 core

const float magnitude = 1.0;

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in O_NormalProj {
    vec3 normal;
} i_np[];

void generate_line(int index) {
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(i_np[index].normal, 0.0) * magnitude;
    EmitVertex();
    EndPrimitive();
}

void main() {
    generate_line(0); 
    generate_line(1);
    generate_line(2);
}

/**
*	几何着色器
*	
*/