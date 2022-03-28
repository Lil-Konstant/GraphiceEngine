#version 410

in vec4 vPosition;
in vec2 vTexCoord;

uniform sampler2D diffuseTexture;

void main()
{
	gl_FragColor = vec4(texture(diffuseTexture, vTexCoord).xyz, 1);
}