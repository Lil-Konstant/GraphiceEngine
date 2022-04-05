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
const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColours[MAX_LIGHTS];
uniform vec3 PointLightPositions[MAX_LIGHTS];

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

vec3 diffuse(vec3 lightDirection, vec3 lightColour, vec3 normal)
{
	return lightColour * max(0, min(1, dot(normal, -lightDirection)));
}

vec3 specular(vec3 lightDirection, vec3 lightColour, vec3 normal, vec3 viewingDisplacement)
{
	vec3 reflectedLight = reflect(lightDirection, normal);
	return lightColour * pow(max(0, dot(reflectedLight, viewingDisplacement)), specularPower);
}

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

	// Calculate the diffuse and specular total starting with the main sunlight
	vec3 diffuseTotal = diffuse(lightDirection, LightColour, normal);
	vec3 viewingDisplacement = normalize(CameraPosition - vWorldPosition);
	vec3 specularTotal = specular(lightDirection, LightColour, normal, viewingDisplacement);
	
	// Iterate through all the point lights, and acculate their diffuse and specular lighting contributions
	for (int i = 0; i < numLights && i < MAX_LIGHTS; i++)
	{
		// Store the direction and distance of this point light
		vec3 direction = vWorldPosition - PointLightPositions[i];
		float distance = length(direction);
		// Normalise the direction now that we have the distance
		direction /= distance;

		// Attenuate the light intensity of this point light with the inverse square law
		vec3 colour = PointLightColours[i]/(distance * distance);

		// Add this point lights diffuse and specular contribution
		diffuseTotal += diffuse(direction, colour, normal);
		specularTotal += specular(direction, colour, normal, viewingDisplacement);
	}

	// Sample the diffuse and specular texture maps using the interpolated UV coord for this fragment
	vec3 diffuseTexColour = texture(diffuseTexture, vTexCoord).rgb;
	vec3 specularTexColour = texture(specularTexture, vTexCoord).rgb;

	// Calculate the three sections of phong lighting, multiplying the appropriate texture colours in
	vec3 ambient = AmbientColour * Ka * diffuseTexColour;
	vec3 diffuse = diffuseTotal * Kd * diffuseTexColour;
	vec3 specular = specularTotal * Ks * specularTexColour;

	

	// combine the each lighting type for the final fragment colour
	gl_FragColor = vec4(ambient + diffuse + specular, 1);
}