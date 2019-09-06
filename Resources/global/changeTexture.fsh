uniform sampler2D baseMap;

varying vec2 v_texCoord;


uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float t;

void main( void )
{ 

    vec4 color1 = texture2D(texture1, v_texCoord);
    vec4 color2 = texture2D(texture2, v_texCoord);
    gl_FragColor = mix(color1, color2, t);
}