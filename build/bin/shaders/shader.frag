#version 330 core

in vec3 col;

out vec3 color;

void main(){

    color = vec3(col.x, col.y, col.z);
}