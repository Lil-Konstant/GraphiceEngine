#version 410

layout (location = 0) in vec4 Position;
out vec4 vPosition;

uniform mat4 ProjectionViewModel;

void main()
{
	vPosition = Position;
	gl_Position = ProjectionViewModel * Position;
}