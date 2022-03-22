#version 410

in vec3 vWorldPosition;
in vec3 vNormal;
in vec2 vTexCoord;

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

// Camera Transform
uniform vec3 CameraPosition;

void main()
{
	vec3 surfaceNormal = normalize(vNormal);
	vec3 lightDirection = normalize(LightDirection);

	// clamp the light calculation between 0 and 1
	float lambertTerm = max(0, min(1, dot(surfaceNormal, -lightDirection)));

	vec3 viewingDisplacement = normalize(CameraPosition - vWorldPosition);
	vec3 reflectedLight = reflect(lightDirection, surfaceNormal);

	float specularTerm = pow(max(0, dot(reflectedLight, viewingDisplacement)), specularPower);

	vec3 ambient = AmbientColour * Ka;
	vec3 diffuse = LightColour * Kd * lambertTerm;
	vec3 specular = LightColour * Ks * specularTerm;

	//ambient = vec3(0.0f, 0.0f, 0.0f);
	//diffuse = vec3(0.0f, 0.0f, 0.0f);
	//specular = vec3(0.0f, 0.0f, 0.0f);
	
	// combine the each lighting type for the final fragment colour
	gl_FragColor = texture(diffuseTexture, vTexCoord) * vec4(ambient + diffuse + specular, 1);
	if (gl_FragColor == vec4(0, 0, 0, 1))
	{
		gl_FragColor = vec4(ambient + diffuse + specular, 1);
	}
}