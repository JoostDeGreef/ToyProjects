#version 150

uniform sampler2D texture;
uniform vec4 inputColor;

void main()
{
    gl_FragColor = vec4(inputColor.rgb, texture2D(texture, gl_TexCoord[0].st).r); 
}
