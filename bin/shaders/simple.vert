#version 410

layout (location = 0) in vec4 Position;
layout (location = 2) in vec2 TexCoord;
out vec4 vPosition;
out vec2 vTexCoord;

uniform mat4 ProjectionViewTransform;
uniform mat4 ModelTransform;

void main()
{
	vPosition = ModelTransform * Position;
	vTexCoord = TexCoord;
	gl_Position = ProjectionViewTransform * ModelTransform * Position;
}