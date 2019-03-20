#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 rgbA;

uniform vec3 camPos;
uniform vec3 cubePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int isWater;

out vec3 col;

void main()
{
   gl_Position = projection*view*model*vec4(aPos, 1.0f);
   
   if (isWater == 1) {
      col = vec3(0, 0, 1);
   } else {
      col = rgbA;
   }
}