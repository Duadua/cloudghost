#version 330 core
out vec4 FragColor;
in vec3 o_color;
 
void main(void) {

    FragColor = vec4(o_color, 1.0f);

}