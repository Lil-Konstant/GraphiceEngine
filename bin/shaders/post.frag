#version 410

in vec2 vTexCoord;
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

void main()
{
	// Scale the texture coordinate to be inset from the quad by half a texel for texture addressing issues
	vec2 texSize = textureSize( renderTexture, 0 ); 
	vec2 texelSize = 1.0f / texSize; 
	vec2 scale = (texSize - texelSize) / texSize; 
	vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

	gl_FragColor = Distort(texCoord);
}