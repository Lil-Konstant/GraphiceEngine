#version 410

/// phong.vert is a standard vertex shader that takes in the 4 OBJMesh 
/// attributes (position, normal, texture coordinate and tangent),
/// and converts the position, normal and tangent into worldspace to
/// pass to the phong.frag shader, calculates the Bi-Tangent at this
/// vertex needed for normal mapping, and simply passes the texture
/// coordinate across unmodified. The shader then also converts the 
/// position attribute into clip-space and passes it to the fragment 
/// shader using the gl_Position keyword variable for drawing.

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
	// Transform the vertex's position, normal and tangent into world space for lighting in frag shader
	vWorldPosition = (ModelTransform * Position).xyz;
	vNormal = (ModelTransform * Normal).xyz;
	vTangent = (ModelTransform * vec4(Tangent.xyz, 0)).xyz;
	// Use the worldspace normal and tangent to construct the worldspace bi-tangent for this point (for normal mapping)
	vBiTangent = cross(vNormal, vTangent) * Tangent.w;

	// Simply pass the texture coordinate to the frag stage for interpolation without modifying it
	vTexCoord = TexCoord;

	// Get the vertex's position in screen space for pixel shading and pass to the frag stage
	gl_Position = ProjectionViewTransform * ModelTransform * Position;
}