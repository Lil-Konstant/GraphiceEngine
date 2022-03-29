#version 410

#define DEFAULT 0
#define BOXBLUR 1
#define CELLSHADE 2

in vec2 vTexCoord;
uniform int selectedPostProcessor;
uniform sampler2D renderTexture;

vec4 Default(vec2 texCoord)
{
	return texture(renderTexture, texCoord);
}

vec4 BoxBlur(vec2 texCoord, vec2 texelSize)
{
	// 9-tap box kernel 
	vec4 colour = texture(renderTexture, texCoord);  
	colour += texture(renderTexture, texCoord + texelSize * vec2(-1,1));  
	colour += texture(renderTexture, texCoord + texelSize * vec2(-1,0));  
	 colour += texture(renderTexture, texCoord + texelSize * vec2(-1,-1));  
	colour += texture(renderTexture, texCoord + texelSize * vec2(0,1));  
	colour += texture(renderTexture, texCoord + texelSize * vec2(0,-1));  
	colour += texture(renderTexture, texCoord + texelSize * vec2(1,1));  
	colour += texture(renderTexture, texCoord + texelSize * vec2(1,0));  
	colour += texture(renderTexture, texCoord + texelSize * vec2(1,-1)); 
 
	return colour / 9;  
}

vec4 Distort(vec2 texCoord) 
{  
	vec2 mid = vec2(0.5f); 
 
	float distanceFromCentre = distance(texCoord, mid); 
	vec2 normalizedCoord = normalize(texCoord - mid); 
	float bias = distanceFromCentre +  
    sin(distanceFromCentre * 15) * 0.05f; 
 
	vec2 newCoord = mid + bias * normalizedCoord; 
	return texture(renderTexture, newCoord); 
}

vec4 SobelEdge(vec2 texCoord, vec2 texelSize)
{
	vec4 colourX = texture(renderTexture, texCoord + texelSize * vec2(-1.0f, 1.0f)) * -1.0f;
	 colourX += texture(renderTexture, texCoord + texelSize * vec2(-1.0f, 0.0f)) * -2.0f;
	 colourX += texture(renderTexture, texCoord + texelSize * vec2(-1.0f, -1.0f)) * -1.0f;
	 colourX += texture(renderTexture, texCoord + texelSize * vec2(1.0f, 1.0f)) * 1.0f;
	 colourX += texture(renderTexture, texCoord + texelSize * vec2(1.0f, 0.0f)) * 2.0f;
	 colourX += texture(renderTexture, texCoord + texelSize * vec2(1.0f, -1.0f)) * 1.0f;

	vec4 colourY = texture(renderTexture, texCoord + texelSize * vec2(-1.0f, 1.0f)) * -1.0f;
	 colourY += texture(renderTexture, texCoord + texelSize * vec2(0.0f, 1.0f)) * -2.0f;
	 colourY += texture(renderTexture, texCoord + texelSize * vec2(1.0f, 1.0f)) * -1.0f;
	 colourY += texture(renderTexture, texCoord + texelSize * vec2(-1.0f, -1.0f)) * 1.0f;
	 colourY += texture(renderTexture, texCoord + texelSize * vec2(0.0f, 1.0f)) * 2.0f;
	 colourY += texture(renderTexture, texCoord + texelSize * vec2(1.0f, -1.0f)) * 1.0f;

	colourX.a = 1.0f;
	colourY.a = 1.0f;
	return sqrt(colourX * colourX + colourY * colourY);
}

void main()
{
	// Scale the texture coordinate to be inset from the quad by half a texel for texture addressing issues
	vec2 texSize = textureSize( renderTexture, 0 ); 
	vec2 texelSize = 1.0f / texSize; 
	vec2 scale = (texSize - texelSize) / texSize; 
	vec2 texCoord = vTexCoord /*/ scale + texelSize * 0.5f*/;

	switch(selectedPostProcessor)
	{
		case DEFAULT:
			gl_FragColor = Default(texCoord);
			break;
		case BOXBLUR:
			gl_FragColor = BoxBlur(texCoord, texelSize);
			break;
		case CELLSHADE:
			vec4 combine = Default(texCoord) - SobelEdge(texCoord, texelSize);
			combine.a = 1.0f;
			gl_FragColor = combine;
			break;
	}
}