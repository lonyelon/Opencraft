#version 330 core

in vec3 col;
in vec2 wSize;

out vec4 color;

void main(){
    const int chWidth = 1;
    const int chLength = 12;

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
        float c = (col.x + col.y + col.z)/3;
        color = vec4(c, c, c, 1);
    } else {
        color = vec4(col.x, col.y, col.z, 1);
    }
}