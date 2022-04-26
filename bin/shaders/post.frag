#version 410

/// post.frag is a specialised fragment shader configured to work with a standard post 
/// processing pipeline, in which it is assumed that the texture coordinate passed
/// is that of a fullscreen quad mesh, and that the render texture uniform is a filled
/// render target that contains the original draw data of the scene for this frame.
/// Based on the selectedPostProcessor chosen in the Application3D's update loop, this
/// shader program will then enact 1 of 6 different post processing logics that access
/// the render texture and manipulate it in some way. This shader is only ever used
/// to draw to a fullscreen quad, and so the effects of the post processing will always
/// appear to perfectly map over the original scene drawing.

// Define the shader options as ints that match their integer values in the ImGUI combo window
#define DEFAULT 0
#define BOXBLUR 1
#define DISTORT 2
#define WATERDISTORT 3
#define INVERT 4
#define EDGEDETECT 5

in vec2 vTexCoord;
uniform float Time;
uniform int selectedPostProcessor;
uniform sampler2D renderTexture;

// ------------------ // Post processing functions start // ------------------ //

/// Default() takes an input of the texture coordinate of this fragment, and
/// simply samples the render texture and returns it unmodified, so as to appear like
/// there is no post processor being applied to the screen.
vec4 Default(vec2 texCoord)
{
    return texture(renderTexture, texCoord);
}

/// BoxBlur() takes an input of the texture coordinate of this fragment, as well
/// as the size of each texel in the render texture, and follows a standard boxblur
/// logic using a 9 box summation kernel surrounding and including the texel for this 
/// fragment. The sum of colours is then divided by 9 and returned to set gl_FragColor, 
/// which has the effect of slightly blurring pixels together and obscuring the original
/// scene image.
vec4 BoxBlur(vec2 texCoord, vec2 texelSize)
{
    vec4 colour = texture(renderTexture, texCoord);  
    colour += texture(renderTexture, texCoord + texelSize * vec2(-1,1));  
    colour += texture(renderTexture, texCoord + texelSize * vec2(-1,0));  
     colour += texture(renderTexture, texCoord + texelSize * vec2(-1,-1));  
    colour += texture(renderTexture, texCoord + texelSize * vec2(0,1));  
    colour += texture(renderTexture, texCoord + texelSize * vec2(0,-1));  
    colour += texture(renderTexture, texCoord + texelSize * vec2(1,1));  
    colour += texture(renderTexture, texCoord + texelSize * vec2(1,0));  
    colour += texture(renderTexture, texCoord + texelSize * vec2(1,-1)); 
    // Average the nine kernel's colours to blur this fragment
    return colour / 9;  
}

/// Distort() takes an input of the texture coordinate of this fragment, and will then
/// alter said coordinate by an amount dependent on it's distance from the centre of
/// the texture, multiplied by the direction of the texture coordinate from the centre.
/// This modified texture coordinate is then used to sample the render texture, with the
/// result returned to set gl_FragColor with. This has the effect of creating a warped
/// circular distortion on the screen, which becomes more severe as you move away from
/// the centre.
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

/// WaterDistort() takes an input of the texture coordinate of this fragment, and will simply
/// modify it's x component by the sin() of it's y component multiplied by the Time uniform.
/// This modified texture coordinate is then used to sample the render texture, with the
/// result returned to set gl_FragColor with. This has the effect of causing a wavey-type
/// pattern to move constantly down the screen in lines over time.
vec4 WaterDistort(vec2 texCoord)
{
    texCoord.x += sin(texCoord.y * 8 * 3.14159 + Time) / 100;
    return texture(renderTexture, texCoord);
}

/// Invert() takes an input of the texture coordinate of this fragment, and will simply
/// sample the render texture using it as normal, but then inverts the colour found for
/// this fragment (by doing 1 - colour) and returns it to set gl_FragColor with.
vec4 Invert(vec2 texCoord)
{
    vec4 colour = texture(renderTexture, texCoord);
    colour = 1 - colour;
    colour.a = 1.0f;
    return colour;
}

/// SobelEdge() takes an input of the texture coordinate of this fragment, as well
/// as the size of each texel in the render texture, and then uses a standard
/// Sobel operator kernel to create an edge detection effect by amplifying harsh
/// colour gradients across the X and Y axis of the screen. This causes the edges
/// of objects to have a thin black line.
vec4 SobelEdge(vec2 texCoord, vec2 texelSize)
{
    // Sum colour across the verticals
    vec4 colourX = texture(renderTexture, texCoord + texelSize * vec2(-1.0f, 1.0f)) * -1.0f;
     colourX += texture(renderTexture, texCoord + texelSize * vec2(-1.0f, 0.0f)) * -2.0f;
     colourX += texture(renderTexture, texCoord + texelSize * vec2(-1.0f, -1.0f)) * -1.0f;
     colourX += texture(renderTexture, texCoord + texelSize * vec2(1.0f, 1.0f)) * 1.0f;
     colourX += texture(renderTexture, texCoord + texelSize * vec2(1.0f, 0.0f)) * 2.0f;
     colourX += texture(renderTexture, texCoord + texelSize * vec2(1.0f, -1.0f)) * 1.0f;

     // Sum colour across the horizontals
    vec4 colourY = texture(renderTexture, texCoord + texelSize * vec2(-1.0f, 1.0f)) * -1.0f;
     colourY += texture(renderTexture, texCoord + texelSize * vec2(0.0f, 1.0f)) * -2.0f;
     colourY += texture(renderTexture, texCoord + texelSize * vec2(1.0f, 1.0f)) * -1.0f;
     colourY += texture(renderTexture, texCoord + texelSize * vec2(-1.0f, -1.0f)) * 1.0f;
     colourY += texture(renderTexture, texCoord + texelSize * vec2(0.0f, 1.0f)) * 2.0f;
     colourY += texture(renderTexture, texCoord + texelSize * vec2(1.0f, -1.0f)) * 1.0f;

    // Make sure the alpha components are still 1
    colourX.a = 1.0f;
    colourY.a = 1.0f;
    return sqrt(colourX * colourX + colourY * colourY); // add the x and y colour gradients in quadrature and return the resultant colour
}

// ------------------ // Post processing functions end // ------------------ //

void main()
{
    // Calculate the texel size as it is used in multiple post processing logic
    vec2 texelSize = 1.0f / textureSize( renderTexture, 0 ); 

    // Apply the appropriate post processor logic based on the option selected in the UI during Application3D update()
    switch(selectedPostProcessor)
    {
        case DEFAULT:
            gl_FragColor = Default(vTexCoord);
            break;
        case BOXBLUR:
            gl_FragColor = BoxBlur(vTexCoord, texelSize);
            break;
        case DISTORT:
            gl_FragColor = Distort(vTexCoord);
            break;
        case WATERDISTORT:
            gl_FragColor = WaterDistort(vTexCoord);
            break;
        case INVERT:
            gl_FragColor = Invert(vTexCoord);
            break;
        case EDGEDETECT:
            vec4 combine = Default(vTexCoord) - SobelEdge(vTexCoord, texelSize);
            combine.a = 1.0f;
            gl_FragColor = combine;
            break;
    }
}