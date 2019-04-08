#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 rgbA;

uniform mat4 view;
uniform mat4 projection;

uniform vec2 windowSize;


out vec3 col;
out vec2 wSize;

void main() {
   gl_Position = projection*view*vec4(aPos, 1.0f);

   col = rgbA;

   wSize = windowSize;
}