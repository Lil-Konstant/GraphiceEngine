#version 410

layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Normal;
out vec3 vWorldPosition;
out vec3 vNormal;

uniform mat4 ProjectionViewTransform;
uniform mat4 ModelTransform;

void main()
{
	vWorldPosition = (ModelTransform * Position).xyz;
	vNormal = (ModelTransform * Normal).xyz;
	gl_Position = ProjectionViewTransform * ModelTransform * Position;
}