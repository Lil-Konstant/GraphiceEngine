#version 410

// Properties passed and interpolated from the vertex stage
in vec3 vWorldPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

// Directional and ambient light properties
uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightDirection;

// Material light properties
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;

// Texture properties
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

// Camera Transform
uniform vec3 CameraPosition;

void main()
{
	vec3 lightDirection = normalize(LightDirection);
	// Make sure the worldspace vertex attributes are normalised
	vec3 normal = normalize(vNormal);
	vec3 tangent = normalize(vTangent);
	vec3 biTangent = normalize(vBiTangent);

	// Construct the tangent basis matrix
	mat3 TBN = mat3(tangent, biTangent, normal);
	// Get the high res normal from the normal map (scale by * 2 - 1 to get from an RGB range to an XYZ normal range)
	vec3 normalTex = texture(normalTexture, vTexCoord).rgb * 2 - 1;
	// Modify the lighting normal based on the high res normal transformed into worldspace
	normal = TBN * normalTex;

	// clamp the light calculation between 0 and 1
	float lambertTerm = max(0, min(1, dot(normal, -lightDirection)));

	vec3 viewingDisplacement = normalize(CameraPosition - vWorldPosition);
	vec3 reflectedLight = reflect(lightDirection, normal);

	float specularTerm = pow(max(0, dot(reflectedLight, viewingDisplacement)), specularPower);

	// Sample the diffuse and specular texture maps using the interpolated UV coord for this fragment
	vec3 diffuseTexColour = texture(diffuseTexture, vTexCoord).rgb;
	vec3 specularTexColour = texture(specularTexture, vTexCoord).rgb;

	// Calculate the three sections of phong lighting, multiplying the appropriate texture colours in
	vec3 ambient = AmbientColour * Ka * diffuseTexColour;
	vec3 diffuse = LightColour * Kd * lambertTerm * diffuseTexColour;
	vec3 specular = LightColour * Ks * specularTerm * specularTexColour;

	// Debug purposes
	//ambient = vec3(0.0f, 0.0f, 0.0f);
	//diffuse = vec3(0.0f, 0.0f, 0.0f);
	//specular = vec3(0.0f, 0.0f, 0.0f);
	
	// combine the each lighting type for the final fragment colour
	
	gl_FragColor = vec4(ambient + diffuse + specular, 1);
	if (gl_FragColor == vec4(0, 0, 0, 1))
	{
		gl_FragColor = vec4(ambient + diffuse + specular, 1);
	}
}