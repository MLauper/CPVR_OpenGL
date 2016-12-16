// gouraud shading fragment shader
// (for one light only and without emissive component)

#version 400

//flat in vec4 vecSurfaceColor;
in vec4 vecSurfaceColor;


vec4 doGouraudShading(void)
{
	return vecSurfaceColor;
}