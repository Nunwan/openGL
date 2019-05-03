#version 330 core
out vec4 FragColor;

in vec2 ourTexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2; 
uniform float opacity;



void main()
{
    FragColor = mix(texture(ourTexture, ourTexCoord),texture(ourTexture2,ourTexCoord),opacity)	;
}