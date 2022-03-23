#version 410

// Attributes enabled by the OBJ loader
layout ( location = 0 ) in vec4 Position;
layout ( location = 1 ) in vec4 Normal;
layout ( location = 2 ) in vec2 TexCoord;
layout ( location = 3 ) in vec4 Tangent;

out vec3 vWorldPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vTangent;
out vec3 vBiTangent;

uniform mat4 ProjectionViewTransform;
uniform mat4 ModelTransform;

void main()
{
	// transform the vertex's position, normal and tangent into world space for lighting in frag shader
	vWorldPosition = (ModelTransform * Position).xyz;
	vNormal = (ModelTransform * Normal).xyz;
	vTangent = (ModelTransform * vec4(Tangent.xyz, 0)).xyz;
	// use the worldspace normal and tangent to construct the worldspace bi-tangent for this point (for normal mapping)
	vBiTangent = cross(vNormal, vTangent) * Tangent.w;

	// simply pass the texture coordinate to the frag stage for interpolation without modifying it
	vTexCoord = TexCoord;

	// get the vertex's position in screen space for pixel shading
	gl_Position = ProjectionViewTransform * ModelTransform * Position;
}