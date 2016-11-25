// texturing fragment shader (using gouraud shading)

#version 400

in vec4 vecSurfaceColor;

layout (location = 0) out vec4 fragColor; // fragment’s final color

void main()
{
	fragColor = vecSurfaceColor;
}