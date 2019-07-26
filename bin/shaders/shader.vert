#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform vec3 selectedCube;

uniform vec2 windowSize;

out vec2 wSize;
out vec2 texCoord;
out vec3 pos;
out vec2 select;

void main() {
   gl_Position = projection*view*model*vec4(aPos, 1.0f);

   wSize = windowSize;

   texCoord = vec2( aTexCoord.x, aTexCoord.y );
   pos = aPos;

   if (
      (round(selectedCube.x) == round(aPos.x - 0.5) || round(selectedCube.x) == round(aPos.x + 0.5))
      && (round(selectedCube.y) == round(aPos.y - 0.5) || round(selectedCube.y) == round(aPos.y + 0.5))
      && (round(selectedCube.z) == round(aPos.z - 0.5) || round(selectedCube.z) == round(aPos.z + 0.5))
   ) {
      select.x = 1;
      select.y = 0;
   } else {
      select.x = 0;
      select.y = 0;
   }
}