#version 330 core

in vec2 wSize;
in vec2 texCoord;
in vec3 pos;
in vec2 select;

uniform sampler2D textureSampler;

out vec4 color;

float vignette() {
    vec2 c = vec2(wSize.x/2, wSize.y/2);
    float x = gl_FragCoord.x;
    float y = gl_FragCoord.y;

    float d = pow(1.3*sqrt( pow(x-c.x, 2) + pow(y-c.y, 2) )/wSize.x, 2);

    return 1-d;
}

void main(){
    const int chWidth = 1;
    const int chLength = 12;
    float augment = pos.y/70;
    if ( augment > 1 ) augment = 1;

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
        color = vec4(fragCol.x*vignette(), fragCol.y*vignette(), fragCol.z*vignette(), 1.0);
        if (select.x == 1) {
            color.x *= 1.2;
            color.y *= 1.2;
            color.z *= 1.2;
        }
    }

    if (color.x > 1) {
        color.x = 1;
    }

    if (color.y > 1) {
        color.y = 1;
    }

    if (color.z > 1) {
        color.z = 1;
    }
}