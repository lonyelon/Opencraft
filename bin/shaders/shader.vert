#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform vec3 selectedCube;

uniform vec2 windowSize;

uniform float cube_size_reduction;

out vec2 wSize;
out vec2 texCoord;
out vec3 pos;
out vec2 select;
out vec2 dist;

void main() {
   gl_Position = projection*view*model*vec4(aPos, 1.0f);

   wSize = windowSize;

   texCoord = vec2( aTexCoord.x, aTexCoord.y );
   pos = aPos;

   if ((round(selectedCube.x*cube_size_reduction) == round(aPos.x*cube_size_reduction - 0.5) || round(selectedCube.x*cube_size_reduction) == round(aPos.x*cube_size_reduction + 0.5))
       && (round(selectedCube.y*cube_size_reduction) == round(aPos.y*cube_size_reduction - 0.5) || round(selectedCube.y*cube_size_reduction) == round(aPos.y*cube_size_reduction + 0.5))
       && (round(selectedCube.z*cube_size_reduction) == round(aPos.z*cube_size_reduction - 0.5) || round(selectedCube.z*cube_size_reduction) == round(aPos.z*cube_size_reduction + 0.5))
   )
       select.x = 1;
   else
       select.x = 0;
}
