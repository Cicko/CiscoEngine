#version 410 core
out vec4 FragColor;

uniform float time;
void main() {
    FragColor = vec4(sin(time), 0.7, 0.7, 1.0); // Teal color
}