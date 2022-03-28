#version 410

layout (location = 0) in vec2 Position;
out vec2 vTexCoord;

void main()
{
	// Calculate the texture coordinate for this point on the fullscreen quad
	vTexCoord = Position * 0.5f + 0.5f;
	// Pass the quads points through with a z of zero and a w of 1
	gl_Position = vec4(Position, 0, 1);
}