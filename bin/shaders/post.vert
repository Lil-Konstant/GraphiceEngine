#version 410

/// post.vert is a specialised vertex shader configured to work
/// with a standard post processing pipeline, in which it is assumed
/// that the attribute Position passed into this shader is one of four 
/// corners of a fullscreen quad already in clip-space. The function 
/// simply calculates the corresponding UV coordinate for the point
/// based on it's position and passes it through to the fragment stage,
/// and then passes the corner Position through without modification
/// to the next stage using the gl_Position keyword.

layout (location = 0) in vec2 Position;
out vec2 vTexCoord;

void main()
{
	// Calculate the texture coordinate for this point on the fullscreen quad
	vTexCoord = Position * 0.5f + 0.5f;
	// Pass the quads points through with a z of zero and a w of 1
	gl_Position = vec4(Position, 0, 1);
}