// texturing fragment shader (using gouraud shading)

#version 400

uniform sampler2D texImage;
uniform bool uReplaceColor;

in vec4 vecSurfaceColor;
in vec4 texCoord;

layout (location = 0) out vec4 fragColor; // fragment’s final color

void main()
{
	// get texture color at x,y position ingnoring alpha value
 	vec3 texColor = vec3(texture(texImage, texCoord.xy, 0));

	if (uReplaceColor)
	{
		// replace material color with texture color 
		float brightness = min(length(vec3(vecSurfaceColor)), 1.0);
		fragColor = vec4(brightness * texColor, 1.0);
	}
	else
	{
		// modulate texture color with material color
		fragColor = vec4(vec3(vecSurfaceColor) * texColor, 1.0);
	}
}