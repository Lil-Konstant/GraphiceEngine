#version 410

/// simple.vert is (as the name suggests) a simple vertex shader
/// that takes in the first two mesh attributes (position and normal),
/// and converts them both into world space to pass to the simple.frag
/// shader. The shader then also converts the position attribute into
/// clip-space and passes it to the fragment shader using the gl_Position
/// keyword variable for drawing.

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