#version 330 core

// ================================================================================
// const 

const float magnitude = 0.4;

// ================================================================================
// in & out

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in O_NormalProj {
    vec3 normal;
} i_np[];

// ================================================================================

void generate_line(int index);

// ================================================================================

void main() {
    generate_line(0); 
    generate_line(1);
    generate_line(2);
}

// ================================================================================

void generate_line(int index) {
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(i_np[index].normal, 0.0) * magnitude;
    EmitVertex();
    EndPrimitive();
}

// ================================================================================

/**
*	几何着色器
*	
*/

// ================================================================================