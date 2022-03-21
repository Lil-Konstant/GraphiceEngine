#version 410

layout ( location = 0 ) in vec4 Position;
layout ( location = 1 ) in vec4 Normal;

out vec3 vWorldPosition;
out vec3 vNormal;

uniform mat4 ProjectionViewTransform;
uniform mat4 ModelTransform; // used to transform the normal from model to world space

void main()
{
	// get the vertex's world position for specular lighting in the frag shader
	vWorldPosition = (ModelTransform * Position).xyz;
	// get the vertex's normal in world space for lighting in frag shader
	vNormal = (ModelTransform * Normal).xyz;
	// get the vertex's position in screen space for pixel shading
	gl_Position = ProjectionViewTransform * ModelTransform * Position;
}