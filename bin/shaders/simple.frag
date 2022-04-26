#version 410

/// simple.frag is (as the name suggests) a simple fragment shader
/// that shades each fragment using it's mesh's material properties
/// combined with a phong lighting model. No textures are used in
/// the "simple" shaders, which allows them to be used for drawing
/// the bunny mesh in the scene (as it is untextured). For lighting,
/// the function first calculates the specular and diffuse contribution
/// from the main directional light, and then iterates through all point
/// lights affecting this mesh object, adding their specular and diffuse
/// contribution to a running total. The shader then finishes by applying
/// the totals to the appropriate ambient/diffuse/specular terms and summing
/// them together as per the phong model.

// Properties passed and interpolated from the vertex stage
in vec3 vWorldPosition; // position of this fragment in worldspace
in vec3 vNormal; // normal of this fragment in worldspace

// Directional and ambient light properties
uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightDirection;
// Point light properties
const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColours[MAX_LIGHTS];
uniform vec3 PointLightPositions[MAX_LIGHTS];

// Material light reflectance properties
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;

// Position of the main camera in the scene
uniform vec3 CameraPosition;

/// diffuse() takes an input of the direction to the light being calculated currently, the colour
/// of said light, as well as the normal of this fragment, and uses them to calculate
/// the lambertian reflectance for this pixel, multiplied by the light colour.
vec3 diffuse(vec3 lightDirection, vec3 lightColour, vec3 normal)
{
	return lightColour * max(0, min(1, dot(normal, -lightDirection)));
}

/// specular() takes an input of the direction to the light being calculated currently, the colour
/// of said light, as well as the normal of this fragment and the viewingDisplacement from the camera, 
/// and uses them to calculate the specular term for this pixel, multiplied by the light colour.
vec3 specular(vec3 lightDirection, vec3 lightColour, vec3 normal, vec3 viewingDisplacement)
{
	vec3 reflectedLight = reflect(lightDirection, normal);
	return lightColour * pow(max(0, dot(reflectedLight, viewingDisplacement)), specularPower);
}

void main()
{
    // Make sure the normal and main light direction are normalised
	vec3 lightDirection = normalize(LightDirection);
	vec3 normal = normalize(vNormal);

	// Calculate the diffuse and specular total starting with the main sunlight
	vec3 diffuseTotal = diffuse(lightDirection, LightColour, normal);
	vec3 viewingDisplacement = normalize(CameraPosition - vWorldPosition);
	vec3 specularTotal = specular(lightDirection, LightColour, normal, viewingDisplacement);
	
	// Iterate through all the point lights, and accumulate their diffuse and specular lighting contributions
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

	// Calculate the three sections of phong lighting, multiplying the appropriate material colours in
	vec3 ambient = AmbientColour * Ka;
	vec3 diffuse = diffuseTotal * Kd;
	vec3 specular = specularTotal * Ks;

	// Combine each lighting section for the final fragment colour
	gl_FragColor = vec4(ambient + diffuse + specular, 1);
}