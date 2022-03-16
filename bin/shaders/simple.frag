#version 410

in vec4 vPosition;
out vec4 FragColour;

uniform float Time;

void main()
{
	FragColour = vec4(vPosition.z * sin(Time * 2f) + 0.3f, vPosition.x * sin(Time * 2f) + 0.3f, vPosition.y * sin(Time * 2f) + 0.3f, 1);
}