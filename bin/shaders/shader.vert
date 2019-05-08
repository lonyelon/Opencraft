#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform vec2 windowSize;

out vec2 wSize;
out vec2 texCoord;
out vec3 pos;

void main() {
   gl_Position = projection*view*model*vec4(aPos, 1.0f);

   wSize = windowSize;

   texCoord = vec2( aTexCoord.x, aTexCoord.y );
   pos = aPos;
}