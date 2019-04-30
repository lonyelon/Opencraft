#version 330 core

in vec2 wSize;
in vec2 texCoord;

uniform sampler2D textureSampler;

out vec4 color;

void main(){
    const int chWidth = 1;
    const int chLength = 12;

    vec4 fragCol = texture( textureSampler, texCoord );

    if ( 
        ( 
            gl_FragCoord.x < wSize.x/2 + chWidth &&
            gl_FragCoord.x > wSize.x/2 - chWidth && 
            gl_FragCoord.y < wSize.y/2 + chLength &&
            gl_FragCoord.y > wSize.y/2 - chLength 
        ) 
        ||
        ( 
            gl_FragCoord.y < wSize.y/2 + chWidth &&
            gl_FragCoord.y > wSize.y/2 - chWidth && 
            gl_FragCoord.x < wSize.x/2 + chLength &&
            gl_FragCoord.x > wSize.x/2 - chLength 
            ) 
    ) {
        float median = (fragCol.x + fragCol.y + fragCol.z)/3;
        color = vec4( 1 - median, 1 - median, 1 - median, 1 );
    } else {
        color = fragCol;
    }
}