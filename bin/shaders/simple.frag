#version 410

// Properties passed and interpolated from the vertex stage
in vec3 vWorldPosition;
in vec3 vNormal;

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

	// Calculate the three sections of phong lighting, multiplying the appropriate texture colours in
	vec3 ambient = AmbientColour * Ka;
	vec3 diffuse = diffuseTotal * Kd;
	vec3 specular = specularTotal * Ks;

	// combine the each lighting type for the final fragment colour
	gl_FragColor = vec4(ambient + diffuse + specular, 1);
}